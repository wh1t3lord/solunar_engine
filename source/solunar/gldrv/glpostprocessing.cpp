#include "gldrv_pch.h"
#include "gldrv/glpostprocessing.h"

#include "graphics/view.h"

#include "graphics/core/device.h"
#include "graphics/core/texture.h"
#include "graphics/core/rendertarget.h"

#include "graphics/ShaderProgramManager.h"
#include "graphics/shaderprogram.h"

#include "graphics/screenquad.h"

#include "glad/glad.h"

namespace engine
{

// global instance
GLPostProcessing g_postProcessing;

const float kBlurTap = 4.0f;

GLPostProcessing::GLPostProcessing()
{

}

GLPostProcessing::~GLPostProcessing()
{
}

void GLPostProcessing::init(View* view)
{
	initHDR(view);
	initBlur(view);
}

void GLPostProcessing::initHDR(View* view)
{
#if 0
	TextureDesc hdrTextureDesc;
	memset(&hdrTextureDesc, 0, sizeof(hdrTextureDesc));
	hdrTextureDesc.m_width = view->m_width;
	hdrTextureDesc.m_height = view->m_height;
	hdrTextureDesc.m_format = ImageFormat::RGBA16F;

	SubresourceDesc hdrTextureSubresourceDesc;
	memset(&hdrTextureSubresourceDesc, 0, sizeof(hdrTextureSubresourceDesc));

	m_hdrTempTex = g_renderDevice->createTexture2D(hdrTextureDesc, hdrTextureSubresourceDesc);

	m_hdrRenderTarget = g_renderDevice->createRenderTarget();
	m_hdrRenderTarget->attachTexture2D(0, m_hdrTempTex);

	g_renderDevice->setRenderTarget(m_hdrRenderTarget);

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Core::error("GLPostProcessing::initHDR: Framebuffer is not complete.");
	}

	g_renderDevice->setRenderTarget(0);

	m_hdrPassProgram = g_shaderManager->createShaderProgram("hdr_main", "shaders/quad.vsh", "shaders/hdr_main.fsh");
#endif
}


void GLPostProcessing::initBlur(View* view)
{
#if 0
	// UE3 weights
	float gaussianWeight1 = 0.54684;
	float gaussianWeight2 = 0.34047;
	float gaussianWeight3 = 0.11269;

	// my weights
	//float gaussianWeight1 = 0.27407;
	//float gaussianWeight2 = 0.45186;
	//float gaussianWeight3 = 0.27407;

	m_blurWeights.push_back(glm::vec4(glm::vec3(gaussianWeight1), 0.33333));
	m_blurWeights.push_back(glm::vec4(glm::vec3(gaussianWeight2), 0.33333));
	m_blurWeights.push_back(glm::vec4(glm::vec3(gaussianWeight3), 0.33333));

	memset(&m_hdrPingPongTextureDesc, 0, sizeof(m_hdrPingPongTextureDesc));
	m_hdrPingPongTextureDesc.m_width = view->m_width / 2;
	m_hdrPingPongTextureDesc.m_height = view->m_height / 2;
	m_hdrPingPongTextureDesc.m_format = ImageFormat::RGBA16F;

	// size of one texel of screen texture
	m_texOffset = glm::vec2(1.0f) / glm::vec2(m_hdrPingPongTextureDesc.m_width, m_hdrPingPongTextureDesc.m_height);

	SubresourceDesc hdrPingPongTextureSubresourceDesc;
	memset(&hdrPingPongTextureSubresourceDesc, 0, sizeof(hdrPingPongTextureSubresourceDesc));

	for (unsigned int i = 0; i < 2; i++)
	{
		// texture creation
		m_hdrPinPingTextures[i] = g_renderDevice->createTexture2D(m_hdrPingPongTextureDesc, hdrPingPongTextureSubresourceDesc);

		// framebuffer creation
		m_hdrPinPongRenderTargets[i] = g_renderDevice->createRenderTarget();
		m_hdrPinPongRenderTargets[i]->attachTexture2D(0, m_hdrPinPingTextures[i]);

		g_renderDevice->setRenderTarget(m_hdrPinPongRenderTargets[i]);

		GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			Core::error("GLPostProcessing::initBlur: Framebuffer is not complete.");
		}

		g_renderDevice->setRenderTarget(0);

	}

	SamplerDesc hdrPinPongSamplerDesc;
	memset(&hdrPinPongSamplerDesc, 0, sizeof(hdrPinPongSamplerDesc));
	hdrPinPongSamplerDesc.m_minFilter = TextureFilter::Nearest;
	hdrPinPongSamplerDesc.m_magFilter = TextureFilter::Nearest;
	hdrPinPongSamplerDesc.m_wrapS = TextureWrap::ClampToEdge;
	hdrPinPongSamplerDesc.m_wrapT = TextureWrap::ClampToEdge;
	hdrPinPongSamplerDesc.m_wrapRepeat = TextureWrap::ClampToEdge;
	hdrPinPongSamplerDesc.m_anisotropyLevel = 1.0f;
	m_hdrPinPongSampler = g_renderDevice->createSamplerState(hdrPinPongSamplerDesc);

	m_blurPassProgram = g_shaderManager->createShaderProgram("hdr_blur", "shaders/quad.vsh", "shaders/hdr_blur.fsh");
	m_blurWeightsLoc = m_blurPassProgram->getUniformLocation("weight");

	m_hdrCombineProgram = g_shaderManager->createShaderProgram("hdr_combine", "shaders/quad.vsh", "shaders/hdr_combine.fsh");
#endif
}

void GLPostProcessing::shutdown()
{
#if 0
	if (m_hdrPinPongSampler)
	{

		mem_delete(m_hdrPinPongSampler);
		m_hdrPinPongSampler = nullptr;
	}

	for (int i = 0; i < 2; i++)
	{
		mem_delete(m_hdrPinPongRenderTargets[i]);
		m_hdrPinPongRenderTargets[i] = nullptr;

		mem_delete(m_hdrPinPingTextures[i]);
		m_hdrPinPingTextures[i] = nullptr;
	}

	mem_delete(m_hdrRenderTarget);
	m_hdrRenderTarget = nullptr;

	mem_delete(m_hdrTempTex);
	m_hdrTempTex = nullptr;
#endif
}

void GLPostProcessing::hdrPass(Texture2DBase* screenTexture)
{
#if 0
	////////////////////////
	// lets go
	////////////////////////

	// setup device state
	g_renderDevice->setRenderTarget(m_hdrRenderTarget);

	/////////////////
	// run first pass
	/////////////////
	g_renderDevice->setTexture2D(0, screenTexture);
	//m_hdrPassProgram->setTextureSampler(0, "u_texture");
	ScreenQuad::renderWithoutTextureBinding(m_hdrPassProgram);

	//////////////////
	// run second pass
	//////////////////
	blurPass(screenTexture);

	///////////////
	// combine pass
	///////////////
	combinePass(screenTexture);
#endif
}

void GLPostProcessing::blurPass(Texture2DBase* screenTexture)
{
#if 0
	Viewport oldViewPort = g_renderDevice->getViewport();

	Viewport blurPassViewport = { 0 };
	blurPassViewport.m_x = 0;
	blurPassViewport.m_y = 0;
	blurPassViewport.m_width = m_hdrPingPongTextureDesc.m_width;
	blurPassViewport.m_height = m_hdrPingPongTextureDesc.m_height;
	g_renderDevice->setViewport(&blurPassViewport);

	// #TODO: too slow(TM) pass
	bool horizontal = true, first_iteration = true;
	int amount = 4;
	for (unsigned int i = 0; i < amount; i++)
	{
		g_renderDevice->setRenderTarget(m_hdrPinPongRenderTargets[horizontal]);

		//glClear(GL_COLOR_BUFFER_BIT);

		ShaderProgramManager::setShaderProgram(m_blurPassProgram);
		m_blurPassProgram->setTextureSampler(0, "u_texture");
		m_blurPassProgram->setInteger("u_horizontal", horizontal);
		glUniform4fv(m_blurWeightsLoc, 3, (float*)m_blurWeights.data());
		m_blurPassProgram->setVector2("texOffset", m_texOffset);

		g_renderDevice->setSampler(0, m_hdrPinPongSampler);
		g_renderDevice->setTexture2D(0, first_iteration ? m_hdrTempTex : m_hdrPinPingTextures[!horizontal]);

		ScreenQuad::renderWithoutTextureBinding(m_blurPassProgram);

		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}

	g_renderDevice->setViewport(&oldViewPort);
#endif
}

void GLPostProcessing::combinePass(Texture2DBase* screenTexture)
{
#if 0
	g_renderDevice->setRenderTarget(0);

	// clear color
	//graphicsDevice->clear(ClearRenderTarget | ClearDepth);

	ShaderProgramManager::setShaderProgram(m_hdrCombineProgram);

	// screen texture
	g_renderDevice->setSampler(0, m_hdrPinPongSampler);
	g_renderDevice->setTexture2D(0, screenTexture);
	m_hdrCombineProgram->setInteger("u_hdrTexture", 0);

	// bloom texture
	g_renderDevice->setSampler(1, m_hdrPinPongSampler);
	g_renderDevice->setTexture2D(1, m_hdrPinPingTextures[0]);
	m_hdrCombineProgram->setInteger("u_bloomTexture", 1);

	// render screen quad
	ScreenQuad::renderWithoutTextureBinding(m_hdrCombineProgram);
#endif
}

}