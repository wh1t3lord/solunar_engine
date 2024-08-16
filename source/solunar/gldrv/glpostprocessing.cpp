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

namespace solunar
{

struct GLHDRConstantBuffer
{
	glm::vec4 weight[3];
	glm::vec4 texOffset;
};

struct GLHDRCombinePassConstants
{

};

static GLHDRConstantBuffer s_GLHDRConstantBuffer;

// global instance
GLPostProcessing g_postProcessing;

const float kBlurTap = 4.0f;

GLPostProcessing::GLPostProcessing()
{

}

GLPostProcessing::~GLPostProcessing()
{
}

void GLPostProcessing::Init(View* view)
{
	initHDR(view);
	initBlur(view);
}

void GLPostProcessing::initHDR(View* view)
{
	TextureDesc hdrTextureDesc;
	memset(&hdrTextureDesc, 0, sizeof(hdrTextureDesc));
	hdrTextureDesc.m_width = view->m_width;
	hdrTextureDesc.m_height = view->m_height;
	hdrTextureDesc.m_format = ImageFormat::RGBA16F;

	SubresourceDesc hdrTextureSubresourceDesc;
	memset(&hdrTextureSubresourceDesc, 0, sizeof(hdrTextureSubresourceDesc));

	m_hdrTempTex = g_renderDevice->CreateTexture2D(hdrTextureDesc, hdrTextureSubresourceDesc);

	RenderTargetCreationDesc hdrRTDesc;
	memset(&hdrRTDesc, 0, sizeof(hdrRTDesc));
	hdrRTDesc.m_textures2D[0] = m_hdrTempTex;
	hdrRTDesc.m_textures2DCount = 1;
	m_hdrRenderTarget = g_renderDevice->CreateRenderTarget(hdrRTDesc);

	m_hdrPassProgram = g_shaderManager->CreateShaderProgram("quad.vsh", "hdr_main.psh");

	BufferDesc constantBufferDesc;
	memset(&constantBufferDesc, 0, sizeof(constantBufferDesc));
	constantBufferDesc.m_bufferAccess = BufferAccess::Stream;
	constantBufferDesc.m_bufferType = BufferType::ConstantBuffer;
	constantBufferDesc.m_bufferMemorySize = sizeof(GLHDRConstantBuffer);

	SubresourceDesc constantSubresourceDesc;
	memset(&constantSubresourceDesc, 0, sizeof(constantSubresourceDesc));
	constantSubresourceDesc.m_memory = &s_GLHDRConstantBuffer;

	m_hdrConstantBuffer = g_renderDevice->CreateBuffer(constantBufferDesc, constantSubresourceDesc);
}

void GLPostProcessing::initBlur(View* view)
{
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
		m_hdrPinPingTextures[i] = g_renderDevice->CreateTexture2D(m_hdrPingPongTextureDesc, hdrPingPongTextureSubresourceDesc);

		// framebuffer creation
		RenderTargetCreationDesc hdrPingPongRTDesc;
		memset(&hdrPingPongRTDesc, 0, sizeof(hdrPingPongRTDesc));
		hdrPingPongRTDesc.m_textures2D[0] = m_hdrPinPingTextures[i];
		hdrPingPongRTDesc.m_textures2DCount = 1;

		m_hdrPinPongRenderTargets[i] = g_renderDevice->CreateRenderTarget(hdrPingPongRTDesc);
	}

	SamplerDesc hdrPinPongSamplerDesc;
	memset(&hdrPinPongSamplerDesc, 0, sizeof(hdrPinPongSamplerDesc));
	hdrPinPongSamplerDesc.m_minFilter = TextureFilter::Nearest;
	hdrPinPongSamplerDesc.m_magFilter = TextureFilter::Nearest;
	hdrPinPongSamplerDesc.m_wrapS = TextureWrap::ClampToEdge;
	hdrPinPongSamplerDesc.m_wrapT = TextureWrap::ClampToEdge;
	hdrPinPongSamplerDesc.m_wrapRepeat = TextureWrap::ClampToEdge;
	hdrPinPongSamplerDesc.m_anisotropyLevel = 1.0f;
	m_hdrPinPongSampler = g_renderDevice->CreateSamplerState(hdrPinPongSamplerDesc);

	m_blurPassProgram = g_shaderManager->CreateShaderProgram("quad.vsh", "hdr_blur.psh");
	m_hdrCombineProgram = g_shaderManager->CreateShaderProgram("quad.vsh", "hdr_combine.psh");
}

void GLPostProcessing::Shutdown()
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

	mem_delete(m_hdrConstantBuffer);
	m_hdrConstantBuffer = nullptr;

	mem_delete(m_hdrRenderTarget);
	m_hdrRenderTarget = nullptr;

	mem_delete(m_hdrTempTex);
	m_hdrTempTex = nullptr;
}

void GLPostProcessing::HDRPass(ITexture2D* screenTexture)
{
	////////////////////////
	// lets go
	////////////////////////

	// setup device state
	g_renderDevice->SetRenderTarget(m_hdrRenderTarget);

	/////////////////
	// run first pass
	/////////////////
	g_renderDevice->SetTexture2D(0, screenTexture);
	//m_hdrPassProgram->setTextureSampler(0, "u_texture");
	ScreenQuad::RenderWithoutTextureBinding(m_hdrPassProgram);

	//////////////////
	// run second pass
	//////////////////
	blurPass(screenTexture);

	///////////////
	// combine pass
	///////////////
	combinePass(screenTexture);
}

void GLPostProcessing::blurPass(ITexture2D* screenTexture)
{
	Viewport oldViewPort = g_renderDevice->GetViewport();

	Viewport blurPassViewport = { 0 };
	blurPassViewport.m_x = 0;
	blurPassViewport.m_y = 0;
	blurPassViewport.m_width = m_hdrPingPongTextureDesc.m_width;
	blurPassViewport.m_height = m_hdrPingPongTextureDesc.m_height;
	g_renderDevice->SetViewport(&blurPassViewport);

	// #TODO: too slow(TM) pass
	bool horizontal = true, first_iteration = true;
	int amount = 4;
	for (unsigned int i = 0; i < amount; i++)
	{
		g_renderDevice->SetRenderTarget(m_hdrPinPongRenderTargets[horizontal]);

		g_shaderManager->SetShaderProgram(m_blurPassProgram);

		// setup textures
		g_renderDevice->SetSamplerState(0, m_hdrPinPongSampler);
		g_renderDevice->SetTexture2D(0, first_iteration ? m_hdrTempTex : m_hdrPinPingTextures[!horizontal]);

		// update constant buffer
		GLHDRConstantBuffer* constants = (GLHDRConstantBuffer*)m_hdrConstantBuffer->Map(BufferMapping::WriteOnly);
		memcpy(constants->weight, m_blurWeights.data(), sizeof(constants->weight));
		constants->texOffset.x = m_texOffset.x;
		constants->texOffset.y = m_texOffset.y;

		// is flipped? 
		constants->texOffset.w = (horizontal) ? 10.0f : 0.0f;

		m_hdrConstantBuffer->Unmap();

		// setup constant buffer
		g_renderDevice->SetConstantBufferIndex(0, m_hdrConstantBuffer);

		// render
		ScreenQuad::RenderWithoutTextureBinding(m_blurPassProgram);

		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}

	g_renderDevice->SetViewport(&oldViewPort);
}

void GLPostProcessing::combinePass(ITexture2D* screenTexture)
{
	g_renderDevice->SetRenderTarget(0);

	// clear color
	//graphicsDevice->clear(ClearRenderTarget | ClearDepth);

	g_shaderManager->SetShaderProgram(m_hdrCombineProgram);

	// screen texture
	g_renderDevice->SetSamplerState(0, m_hdrPinPongSampler);
	g_renderDevice->SetTexture2D(0, screenTexture);

	// bloom texture
	g_renderDevice->SetSamplerState(1, m_hdrPinPongSampler);
	g_renderDevice->SetTexture2D(1, m_hdrPinPingTextures[0]);

	// render screen quad
	ScreenQuad::RenderWithoutTextureBinding(m_hdrCombineProgram);
}

}