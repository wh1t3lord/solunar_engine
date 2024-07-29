#include "graphicspch.h"
#include "graphics/postfxmanager.h"

#include "graphics/view.h"

#include "graphics/core/device.h"
#include "graphics/core/texture.h"
#include "graphics/core/rendertarget.h"

#include "graphics/ShaderProgramManager.h"
#include "graphics/shaderprogram.h"

#include "graphics/screenquad.h"

#include "graphics/renderer.h"

namespace engine
{

struct HDRConstants
{
	glm::vec4 weight[3];
	glm::vec4 texOffset;
};

// global instance
PostFxManager g_postFxManager;

const float kBlurTap = 4.0f;

PostFxManager::PostFxManager()
{

}

PostFxManager::~PostFxManager()
{
}

void PostFxManager::init(View* view)
{
	initHDR(view);
	initBlur(view);
}

void PostFxManager::initHDR(View* view)
{
	TextureDesc hdrTextureDesc;
	memset(&hdrTextureDesc, 0, sizeof(hdrTextureDesc));
	hdrTextureDesc.m_textureType = TextureType::Texture2D;
	hdrTextureDesc.m_width = view->m_width;
	hdrTextureDesc.m_height = view->m_height;
	hdrTextureDesc.m_format = ImageFormat::RGBA16F;
	hdrTextureDesc.m_renderTargetUsage = true;

	SubresourceDesc hdrTextureSubresourceDesc;
	memset(&hdrTextureSubresourceDesc, 0, sizeof(hdrTextureSubresourceDesc));

	m_hdrTempTex = g_renderDevice->createTexture2D(hdrTextureDesc, hdrTextureSubresourceDesc);

	RenderTargetCreationDesc rtCreationDesc = {};
	rtCreationDesc.m_textures2D[0] = m_hdrTempTex;
	rtCreationDesc.m_textures2DCount = 1;
	m_hdrRenderTarget = g_renderDevice->createRenderTarget(rtCreationDesc);

	m_hdrPassProgram = g_shaderManager->createShaderProgram(
		"quad.vsh", 
		"hdr_main.psh",
		nullptr,
		ScreenQuad::ms_inputLayout,
		sizeof(ScreenQuad::ms_inputLayout) / sizeof(ScreenQuad::ms_inputLayout[0]));
}

void PostFxManager::initBlur(View* view)
{
	// UE3 weights
	float gaussianWeight1 = 0.54684f;
	float gaussianWeight2 = 0.34047f;
	float gaussianWeight3 = 0.11269f;

	// my weights
	//float gaussianWeight1 = 0.27407f;
	//float gaussianWeight2 = 0.45186f;
	//float gaussianWeight3 = 0.27407f;

	m_blurWeights.push_back(glm::vec4(glm::vec3(gaussianWeight1), 0.33333));
	m_blurWeights.push_back(glm::vec4(glm::vec3(gaussianWeight2), 0.33333));
	m_blurWeights.push_back(glm::vec4(glm::vec3(gaussianWeight3), 0.33333));

	memset(&m_hdrPingPongTextureDesc, 0, sizeof(m_hdrPingPongTextureDesc));
	m_hdrPingPongTextureDesc.m_width = view->m_width / 2;
	m_hdrPingPongTextureDesc.m_height = view->m_height / 2;
	m_hdrPingPongTextureDesc.m_format = ImageFormat::RGBA16F;
	m_hdrPingPongTextureDesc.m_renderTargetUsage = true;

	// size of one texel of screen texture
	m_texOffset = glm::vec2(1.0f) / glm::vec2(m_hdrPingPongTextureDesc.m_width, m_hdrPingPongTextureDesc.m_height);

	SubresourceDesc hdrPingPongTextureSubresourceDesc;
	memset(&hdrPingPongTextureSubresourceDesc, 0, sizeof(hdrPingPongTextureSubresourceDesc));

	for (unsigned int i = 0; i < 2; i++)
	{
		// texture creation
		m_hdrPinPingTextures[i] = g_renderDevice->createTexture2D(m_hdrPingPongTextureDesc, hdrPingPongTextureSubresourceDesc);

		// framebuffer creation
		RenderTargetCreationDesc rtCreationDesc = {};
		rtCreationDesc.m_textures2D[0] = m_hdrPinPingTextures[i];
		rtCreationDesc.m_textures2DCount = 1;

		m_hdrPinPongRenderTargets[i] = g_renderDevice->createRenderTarget(rtCreationDesc);
	}

	SamplerDesc hdrPinPongSamplerDesc;
	memset(&hdrPinPongSamplerDesc, 0, sizeof(hdrPinPongSamplerDesc));
	hdrPinPongSamplerDesc.m_minFilter = TextureFilter::Linear;
	hdrPinPongSamplerDesc.m_magFilter = TextureFilter::Linear;
	hdrPinPongSamplerDesc.m_wrapS = TextureWrap::ClampToEdge;
	hdrPinPongSamplerDesc.m_wrapT = TextureWrap::ClampToEdge;
	hdrPinPongSamplerDesc.m_wrapRepeat = TextureWrap::ClampToEdge;
	hdrPinPongSamplerDesc.m_anisotropyLevel = 1.0f;
	m_hdrPinPongSampler = g_renderDevice->createSamplerState(hdrPinPongSamplerDesc);

	m_blurPassProgram = g_shaderManager->createShaderProgram(
		"quad.vsh", 
		"hdr_blur.psh",
		nullptr,
		ScreenQuad::ms_inputLayout,
		sizeof(ScreenQuad::ms_inputLayout) / sizeof(ScreenQuad::ms_inputLayout[0]));

	m_hdrCombineProgram = g_shaderManager->createShaderProgram(
		"quad.vsh", 
		"hdr_combine.psh",
		nullptr,
		ScreenQuad::ms_inputLayout,
		sizeof(ScreenQuad::ms_inputLayout) / sizeof(ScreenQuad::ms_inputLayout[0]));

	m_hdrconstants = ShaderConstantManager::getInstance()->create<HDRConstants>("HDRConstants");
}

void PostFxManager::shutdown()
{
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
}

void PostFxManager::hdrPass(ITexture2D* screenTexture)
{
	// lets go

	// setup device state
	g_renderDevice->setRenderTarget(m_hdrRenderTarget);

	// run first pass
	g_renderDevice->setTexture2D(0, screenTexture);
	ScreenQuad::renderWithoutTextureBinding(m_hdrPassProgram);

	// run second pass
	blurPass(screenTexture);

	// combine pass
	combinePass(screenTexture);
}

void PostFxManager::blurPass(ITexture2D* screenTexture)
{
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
	for (int i = 0; i < amount; i++)
	{
		g_renderDevice->setRenderTarget(m_hdrPinPongRenderTargets[horizontal]);

		//glClear(GL_COLOR_BUFFER_BIT);

		g_shaderManager->setShaderProgram(m_blurPassProgram);

		//m_blurPassProgram->setTextureSampler(0, "u_texture");
		//m_blurPassProgram->setInteger("u_horizontal", horizontal);
		//glUniform4fv(m_blurWeightsLoc, 3, (float*)m_blurWeights.data());
		//m_blurPassProgram->setVector2("texOffset", m_texOffset);

		// #TODO: REMOVE THIS SHIT
		HDRConstants* pHDRConstants = (HDRConstants*)m_hdrconstants->map(BufferMapping::WriteOnly);
		memcpy(pHDRConstants->weight, m_blurWeights.data(), m_blurWeights.size() * sizeof(glm::vec4));
		pHDRConstants->texOffset = glm::vec4(m_texOffset, 0.0f, 0.0f);
		m_hdrconstants->unmap();

		g_renderDevice->setConstantBufferIndex(0, m_hdrconstants.get());
		g_renderDevice->setSampler(0, m_hdrPinPongSampler);
		g_renderDevice->setTexture2D(0, first_iteration ? m_hdrTempTex : m_hdrPinPingTextures[!horizontal]);

		ScreenQuad::renderWithoutTextureBinding(m_blurPassProgram);

		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}

	g_renderDevice->setViewport(&oldViewPort);
}

void PostFxManager::combinePass(ITexture2D* screenTexture)
{
	g_renderer->setSwapChainRenderTarget();

	// clear color
	//graphicsDevice->clear(ClearRenderTarget | ClearDepth);

	g_shaderManager->setShaderProgram(m_hdrCombineProgram);

	// screen texture
	g_renderDevice->setSampler(0, m_hdrPinPongSampler);
	g_renderDevice->setTexture2D(0, screenTexture);
	//m_hdrCombineProgram->setInteger("u_hdrTexture", 0);

	// bloom texture
	g_renderDevice->setSampler(1, m_hdrPinPongSampler);
	g_renderDevice->setTexture2D(1, m_hdrPinPingTextures[0]);
	//m_hdrCombineProgram->setInteger("u_bloomTexture", 1);

	// render screen quad
	ScreenQuad::renderWithoutTextureBinding(m_hdrCombineProgram);
}

}