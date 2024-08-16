#include "graphicspch.h"

// Graphics Core Objects
#include "graphics/core/texture.h"
#include "graphics/core/device.h"
#include "graphics/core/rendertarget.h"

// Graphics classes
#include "graphics/renderer.h"
#include "graphics/shadowsrenderer.h"
#include "graphics/shaderprogram.h"
#include "graphics/ShaderProgramManager.h"
#include "graphics/graphicsworld.h"
#include "graphics/mesh.h"
#include "graphics/light.h"
#include "graphics/lightmanager.h"

namespace solunar
{
	ShadowsRenderer ShadowsRenderer::ms_instance;

	ShadowsRenderer::ShadowsRenderer()
	{
		m_shadowShader_StaticMesh = nullptr;
		memset(&m_originalViewport, 0, sizeof(m_originalViewport));
	}

	ShadowsRenderer::~ShadowsRenderer()
	{
		m_shadowShader_StaticMesh = nullptr;
		memset(&m_originalViewport, 0, sizeof(m_originalViewport));
	}

	const int kShadowMapSize = 1024;

	void ShadowsRenderer::Init()
	{
		TextureDesc textureDesc;
		memset(&textureDesc, 0, sizeof(textureDesc));
		textureDesc.m_textureType = TextureType::Texture2D;
		textureDesc.m_width = textureDesc.m_height = kShadowMapSize;
		textureDesc.m_mipmapLevel = 1;
		textureDesc.m_format = ImageFormat::DEPTH32F;
		textureDesc.m_renderTargetUsage = true;

		SubresourceDesc subresourceDesc;
		memset(&subresourceDesc, 0, sizeof(subresourceDesc));

		// Create shadow map texture
		m_shadowMap = g_renderDevice->CreateTexture2D(textureDesc, subresourceDesc);

		RenderTargetCreationDesc renderTargetDesc;
		memset(&renderTargetDesc, 0, sizeof(renderTargetDesc));
		renderTargetDesc.m_depthTexture2D = m_shadowMap;

		// Create render target view
		m_shadowFbo = g_renderDevice->CreateRenderTarget(renderTargetDesc);

		// Create shader for static mesh
		m_shadowShader_StaticMesh = g_shaderManager->CreateShaderProgram("shadowmap.hlsl", "shadowmap.hlsl");
	}

	void ShadowsRenderer::Shutdown()
	{
		if (m_shadowMap)
		{
			mem_delete(m_shadowMap);
			m_shadowMap = nullptr;
		}

		if (m_shadowFbo)
		{
			mem_delete(m_shadowFbo);
			m_shadowFbo = nullptr;
		}
	}

	void ShadowsRenderer::beginRender()
	{
		m_originalViewport = g_renderDevice->GetViewport();

		// set framebuffer
		g_renderDevice->SetRenderTarget(m_shadowFbo);

		// install shadowmap viewport
		Viewport vp;
		vp.m_x = 0;
		vp.m_y = 0;
		vp.m_width = vp.m_height = kShadowMapSize;
		g_renderDevice->SetViewport(&vp);
	}

	void ShadowsRenderer::renderMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh)
	{
		LightManager* lightManager = graphicsWorld->GetLightManager();
		Assert(lightManager); // uninitialized light manager, critical error

		// Get directional light
		DirectionalLightComponent* directionalLight = lightManager->GetDirectionalLight();
		if (!directionalLight)
			return;

		// Get directional light entity
		Entity* directionalLightEntity = directionalLight->GetEntity();
		if (!directionalLightEntity)
			return;

		// calculate view matrix for light
		//glm::mat4 lightViewMatrix = glm::lookAt(  )
	}

	void ShadowsRenderer::endRender()
	{
		// set framebuffer
		g_renderDevice->SetRenderTarget(g_renderer->getSwapChainRenderTarget());

		// restore viewport
		g_renderDevice->SetViewport(&m_originalViewport);
	}

}