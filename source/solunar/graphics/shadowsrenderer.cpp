#include "graphicspch.h"

// Graphics Core Objects
#include "graphics/core/texture.h"
#include "graphics/core/device.h"
#include "graphics/core/rendertarget.h"

// Graphics classes
#include "graphics/rendercontext.h"
#include "graphics/renderer.h"
#include "graphics/shadowsrenderer.h"
#include "graphics/shaderprogram.h"
#include "graphics/ShaderProgramManager.h"
#include "graphics/shaderconstantmanager.h"
#include "graphics/graphicsworld.h"
#include "graphics/mesh.h"
#include "graphics/light.h"
#include "graphics/lightmanager.h"
#include "graphics/animatedmodel.h"
#include "graphics/screenquad.h"

namespace engine
{
	ShadowsRenderer ShadowsRenderer::ms_instance;

	ShadowsRenderer::ShadowsRenderer()
	{
		m_shadowMap = nullptr;
		m_shadowFbo = nullptr;
		m_shadowShader_StaticMesh = nullptr;
		m_shadowShader_AnimationMesh = nullptr;
		memset(&m_originalViewport, 0, sizeof(m_originalViewport));
	}

	ShadowsRenderer::~ShadowsRenderer()
	{
		m_shadowShader_StaticMesh = nullptr;
		memset(&m_originalViewport, 0, sizeof(m_originalViewport));
	}

	const int kShadowMapSize = 1024;

	void ShadowsRenderer::init()
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
		m_shadowMap = g_renderDevice->createTexture2D(textureDesc, subresourceDesc);

		RenderTargetCreationDesc renderTargetDesc;
		memset(&renderTargetDesc, 0, sizeof(renderTargetDesc));
		renderTargetDesc.m_depthTexture2D = m_shadowMap;

		// Create render target view
		m_shadowFbo = g_renderDevice->createRenderTarget(renderTargetDesc);

		// Create shader for static mesh
		m_shadowShader_StaticMesh = g_shaderManager->createShaderProgram(
			"shadowmap.hlsl",
			"shadowmap.hlsl",
			nullptr,
			g_vertexInputLayout,
			sizeof(g_vertexInputLayout) / sizeof(g_vertexInputLayout[0]));

		// Create shader for animated mesh
		m_shadowShader_AnimationMesh = g_shaderManager->createShaderProgram(
			"shadowmap.hlsl",
			"shadowmap.hlsl",
			"SKINNED\n",
			g_vertexInputLayout,
			sizeof(g_vertexInputLayout) / sizeof(g_vertexInputLayout[0]));
	}

	void ShadowsRenderer::shutdown()
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
		m_originalViewport = g_renderDevice->getViewport();

		// set framebuffer
		g_renderDevice->setRenderTarget(m_shadowFbo);

		// install shadowmap viewport
		Viewport vp;
		vp.m_x = 0;
		vp.m_y = 0;
		vp.m_width = vp.m_height = kShadowMapSize;
		g_renderDevice->setViewport(&vp);
	}

	void ShadowsRenderer::renderMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh)
	{
		LightManager* lightManager = graphicsWorld->getLightManager();
		Assert(lightManager); // uninitialized light manager, critical error

		// Get directional light
		DirectionalLightComponent* directionalLight = lightManager->getDirectionalLight();
		if (!directionalLight)
			return;

		// Get directional light entity
		Entity* directionalLightEntity = directionalLight->getEntity();
		if (!directionalLightEntity)
			return;

		float near_plane = 1.0f, far_plane = 7.5f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

		// calculate view matrix for light
		glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), 
										  glm::vec3( 0.0f, 0.0f,  0.0f), 
										  glm::vec3( 0.0f, 1.0f,  0.0f));

		RenderContext renderContext;
		renderContext.width = kShadowMapSize;
		renderContext.height = kShadowMapSize;
		renderContext.proj = lightProjection;
		renderContext.view = lightView;
		renderContext.model = directionalLightEntity->getWorldTranslation();
		ShaderConstantManager::getInstance()->setStaticMeshGlobalData(mesh, view, renderContext, graphicsWorld);

		if (mesh->isA<AnimatedMeshComponent>())
			AnimatedModelRenderer::getInstance()->render(dynamicCast<AnimatedMeshComponent>(mesh));

		// shader selection
		IShaderProgram* shaderProgram = nullptr;
		if (mesh->isA<AnimatedMeshComponent>())
			shaderProgram = m_shadowShader_AnimationMesh;
		else
			shaderProgram = m_shadowShader_StaticMesh;

		g_shaderManager->setShaderProgram(shaderProgram);

		if (mesh->isA<AnimatedMeshComponent>())
		{
			std::shared_ptr<ModelBase> model = mesh->lockModel();
			AnimatedModel* animatedModel = dynamicCast<AnimatedModel>(model.get());

			for (const auto& submesh : animatedModel->getAnimatedSubmehes())
			{
				// transpose matrices for D3D11
				//localCtx.model = glm::transpose(localCtx.model);

				g_renderDevice->setVertexBuffer(submesh->m_vertexBuffer, sizeof(AnimatedVertex), 0);
				g_renderDevice->setIndexBuffer(submesh->m_indexBuffer, false);

				ShaderConstantManager::getInstance()->setStaticMeshGlobalData(mesh, view, renderContext, graphicsWorld);

				g_renderDevice->drawIndexed(PM_TriangleList, 0, submesh->m_indicesCount, 0);
			}
		}
		else
		{
			std::shared_ptr<ModelBase> model = mesh->lockModel();

			for (const auto& submesh : model->getSubmehes())
			{
				// create saved render ctx as previous model.
				RenderContext savedCtx = renderContext;

				// create local copy of render context
				RenderContext localCtx = savedCtx;

				// and overwrite model matrix
				localCtx.model = savedCtx.model * submesh->getTransform();

				// transpose matrices for D3D11
				//localCtx.model = glm::transpose(localCtx.model);

				// set our local render ctx
				RenderContext::setContext(localCtx);

				g_renderDevice->setVertexBuffer(submesh->getVertexBuffer(), sizeof(Vertex), 0);

				ShaderConstantManager::getInstance()->setStaticMeshGlobalData(mesh, view, localCtx, graphicsWorld);
				
				g_renderDevice->draw(PM_TriangleList, 0, submesh->getVerticesCount());

				// return what have been
				renderContext = savedCtx;
			}
		}
	}

	void ShadowsRenderer::endRender()
	{
		// set framebuffer
		g_renderDevice->setRenderTarget(g_renderer->getSwapChainRenderTarget());

		// restore viewport
		g_renderDevice->setViewport(&m_originalViewport);

		// draw shadow map
		ScreenQuad::render(m_shadowMap);
	}

}