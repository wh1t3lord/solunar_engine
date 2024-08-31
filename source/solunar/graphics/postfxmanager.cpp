#include "graphicspch.h"
#include "graphics/postfxmanager.h"

#include "graphics/view.h"

#include "graphics/core/device.h"
#include "graphics/core/texture.h"
#include "graphics/core/rendertarget.h"
#include "graphics/core/statemanager.h"

#include "graphics/ShaderProgramManager.h"
#include "graphics/shaderprogram.h"

#include "graphics/screenquad.h"

#include "graphics/renderer.h"

namespace solunar
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

void PostFxManager::Init(View* view)
{
	initHDR(view);
	initBlur(view);

	BlendStateDesc blendState = {};
	blendState.m_renderTarget[0].m_blendEnable = false;
	blendState.m_renderTarget[0].m_srcBlend = BLEND_SRC_COLOR;
	blendState.m_renderTarget[0].m_destBlend = BLEND_ZERO;
	blendState.m_renderTarget[0].m_blendOp = BLEND_OP_ADD;
	blendState.m_renderTarget[0].m_srcBlendAlpha = BLEND_ZERO;
	blendState.m_renderTarget[0].m_destBlendAlpha = BLEND_ONE;
	blendState.m_renderTarget[0].m_blendOpAlpha = BLEND_OP_ADD;
	blendState.m_renderTarget[0].m_renderTargetWriteMask = COLOR_WRITE_ENABLE_ALL;
	m_blendState = g_stateManager->CreateBlendState(blendState);
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

	m_hdrTempTex = g_renderDevice->CreateTexture2D(hdrTextureDesc, hdrTextureSubresourceDesc);
	m_hdrTempTex->SetDebugName("HDR Temp Texture");

	RenderTargetCreationDesc rtCreationDesc = {};
	rtCreationDesc.m_textures2D[0] = m_hdrTempTex;
	rtCreationDesc.m_textures2DCount = 1;
	m_hdrRenderTarget = g_renderDevice->CreateRenderTarget(rtCreationDesc);
	m_hdrRenderTarget->SetDebugName("HDR Render Target");

	m_hdrPassProgram = g_shaderManager->CreateShaderProgram(
		"quad.vsh", 
		"hdr_main.psh",
		nullptr,
		ScreenQuad::ms_inputLayout,
		sizeof(ScreenQuad::ms_inputLayout) / sizeof(ScreenQuad::ms_inputLayout[0]));
}

void PostFxManager::initBlur(View* view)
{
	char buf[64];

	// UE3 weights
	//float gaussianWeight1 = 0.54684f;
	//float gaussianWeight2 = 0.34047f;
	//float gaussianWeight3 = 0.11269f;

	// my weights
	float gaussianWeight1 = 0.27407f;
	float gaussianWeight2 = 0.45186f;
	float gaussianWeight3 = 0.27407f;

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

	for (int i = 0; i < 2; i++)
	{
		// texture creation
		m_hdrPinPongTextures[i] = g_renderDevice->CreateTexture2D(m_hdrPingPongTextureDesc, hdrPingPongTextureSubresourceDesc);

		snprintf(buf, sizeof(buf), "HDR Pin-Pong Texture %i", i);
		m_hdrPinPongTextures[i]->SetDebugName(buf);

		// framebuffer creation
		RenderTargetCreationDesc rtCreationDesc = {};
		rtCreationDesc.m_textures2D[0] = m_hdrPinPongTextures[i];
		rtCreationDesc.m_textures2DCount = 1;

		m_hdrPinPongRenderTargets[i] = g_renderDevice->CreateRenderTarget(rtCreationDesc);

		snprintf(buf, sizeof(buf), "HDR Pin-Pong Render Target %i", i);
		m_hdrPinPongRenderTargets[i]->SetDebugName(buf);
	}

	SamplerDesc hdrPinPongSamplerDesc;
	memset(&hdrPinPongSamplerDesc, 0, sizeof(hdrPinPongSamplerDesc));
	hdrPinPongSamplerDesc.m_minFilter = TextureFilter::LinearMipmapLinear;
	hdrPinPongSamplerDesc.m_magFilter = TextureFilter::Linear;
	hdrPinPongSamplerDesc.m_wrapS = TextureWrap::ClampToEdge;
	hdrPinPongSamplerDesc.m_wrapT = TextureWrap::ClampToEdge;
	hdrPinPongSamplerDesc.m_wrapRepeat = TextureWrap::ClampToEdge;
	hdrPinPongSamplerDesc.m_anisotropyLevel = 1.0f;
	m_hdrPinPongSampler = g_renderDevice->CreateSamplerState(hdrPinPongSamplerDesc);

	m_blurPassProgram = g_shaderManager->CreateShaderProgram(
		"quad.vsh", 
		"hdr_blur.psh",
		nullptr,
		ScreenQuad::ms_inputLayout,
		sizeof(ScreenQuad::ms_inputLayout) / sizeof(ScreenQuad::ms_inputLayout[0]));

	m_blurPassHorizontalProgram = g_shaderManager->CreateShaderProgram(
		"quad.vsh",
		"hdr_blur.psh",
		"BLUR_HORIZONTAL\n",
		ScreenQuad::ms_inputLayout,
		sizeof(ScreenQuad::ms_inputLayout) / sizeof(ScreenQuad::ms_inputLayout[0]));

	m_hdrCombineProgram = g_shaderManager->CreateShaderProgram(
		"quad.vsh", 
		"hdr_combine.psh",
		nullptr,
		ScreenQuad::ms_inputLayout,
		sizeof(ScreenQuad::ms_inputLayout) / sizeof(ScreenQuad::ms_inputLayout[0]));

	m_hdrconstants = ShaderConstantManager::GetInstance()->Create<HDRConstants>("HDRConstants");
}

void PostFxManager::Shutdown()
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

		mem_delete(m_hdrPinPongTextures[i]);
		m_hdrPinPongTextures[i] = nullptr;
	}

	mem_delete(m_hdrRenderTarget);
	m_hdrRenderTarget = nullptr;

	mem_delete(m_hdrTempTex);
	m_hdrTempTex = nullptr;
}

void PostFxManager::HDRPass(ITexture2D* screenTexture)
{
	// lets go
	const float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
	g_stateManager->SetBlendState(m_blendState, blend_factor, 0xffffffff);

	// setup device state
	g_renderDevice->SetRenderTarget(m_hdrRenderTarget);

	// run first pass (copy texture)
	g_renderDevice->SetTexture2D(0, screenTexture);
	ScreenQuad::RenderWithoutTextureBinding(m_hdrPassProgram);

	// run second pass (blur)
	blurPass(screenTexture);

	// combine pass (combine)
	combinePass(screenTexture);
}

void PostFxManager::blurPass(ITexture2D* screenTexture)
{
	// #TODO: REMOVE THIS SHIT
	HDRConstants* pHDRConstants = (HDRConstants*)m_hdrconstants->Map(BufferMapping::WriteOnly);
	memcpy(pHDRConstants->weight, m_blurWeights.data(), m_blurWeights.size() * sizeof(glm::vec4));
	pHDRConstants->texOffset = glm::vec4(m_texOffset, 0.0f, 0.0f);
	m_hdrconstants->Unmap();

	g_renderDevice->SetConstantBufferIndex(0, m_hdrconstants.get());

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
	for (int i = 0; i < amount; i++)
	{
		g_renderDevice->SetRenderTarget(m_hdrPinPongRenderTargets[horizontal]);

		g_renderDevice->SetTexture2D(0, first_iteration ? m_hdrTempTex : m_hdrPinPongTextures[!horizontal]);
		g_renderDevice->SetSamplerState(0, m_hdrPinPongSampler);

		IShaderProgram* shader = horizontal ? m_blurPassHorizontalProgram : m_blurPassProgram;
		ScreenQuad::RenderWithoutTextureBinding(shader);

		horizontal = !horizontal;

		if (first_iteration)
			first_iteration = false;
	}

	g_renderDevice->SetViewport(&oldViewPort);
}

void PostFxManager::combinePass(ITexture2D* screenTexture)
{
	g_renderer->setSwapChainRenderTarget();

	// screen texture
	g_renderDevice->SetTexture2D(0, screenTexture);
	g_renderDevice->SetSamplerState(0, m_hdrPinPongSampler);

	// bloom texture
	g_renderDevice->SetTexture2D(1, m_hdrPinPongTextures[0]);
	g_renderDevice->SetSamplerState(1, m_hdrPinPongSampler);

	// render screen quad
	ScreenQuad::RenderWithoutTextureBinding(m_hdrCombineProgram);
}

}