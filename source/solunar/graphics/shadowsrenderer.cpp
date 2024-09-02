#include "graphicspch.h"

// Graphics Core Objects
#include "graphics/core/texture.h"
#include "graphics/core/device.h"
#include "graphics/core/rendertarget.h"
#include "graphics/core/statemanager.h"

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
#include "graphics/debugrenderer.h"

#include "graphics/imguimanager.h"

namespace solunar
{
	ShadowsRenderer ShadowsRenderer::ms_instance;

	ShadowsRenderer::ShadowsRenderer()
	{
		m_shadowMap = nullptr;
		m_shadowFbo = nullptr;
		m_shadowMapSampler = nullptr;
		m_shadowShader_StaticMesh = nullptr;
		m_shadowShader_AnimationMesh = nullptr;
		memset(&m_originalViewport, 0, sizeof(m_originalViewport));
		m_lightViewMatrix = glm::mat4(1.0f);

		m_znear = 1.0f;
		m_zfar = 16.0f;
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
		m_shadowShader_StaticMesh = g_shaderManager->CreateShaderProgram(
			"shadowmap.hlsl",
			"shadowmap.hlsl",
			nullptr,
			g_vertexInputLayout,
			sizeof(g_vertexInputLayout) / sizeof(g_vertexInputLayout[0]));

		// Create shader for animated mesh
		m_shadowShader_AnimationMesh = g_shaderManager->CreateShaderProgram(
			"shadowmap.hlsl",
			"shadowmap.hlsl",
			"SKINNED\n",
			g_animatedVertexInputLayout,
			sizeof(g_animatedVertexInputLayout) / sizeof(g_animatedVertexInputLayout[0]));

		SamplerDesc samplerDesc = {};
		samplerDesc.m_minFilter = TextureFilter::LinearMipmapLinear;
		samplerDesc.m_magFilter = TextureFilter::Linear;
		samplerDesc.m_wrapS = TextureWrap::ClampToBorder;
		samplerDesc.m_wrapT = TextureWrap::ClampToBorder;
		samplerDesc.m_wrapRepeat = TextureWrap::ClampToBorder;
		samplerDesc.m_comparisonFunc = COMPARISON_LESS_EQUAL;
		m_shadowMapSampler = g_stateManager->CreateSamplerState(samplerDesc);
	}

	void ShadowsRenderer::Shutdown()
	{
		if (m_shadowFbo)
		{
			mem_delete(m_shadowFbo);
			m_shadowFbo = nullptr;
		}

		if (m_shadowMap)
		{
			mem_delete(m_shadowMap);
			m_shadowMap = nullptr;
		}
	}

	void ShadowsRenderer::BeginRender()
	{
		ImGui::DragFloat("Z Near", &m_znear, 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat("Z Far", &m_zfar, 1.0f, 1.0f, 100.0f);

		m_originalViewport = g_renderDevice->GetViewport();

		// set framebuffer
		g_renderDevice->SetRenderTarget(m_shadowFbo);

		// clear target
		g_renderer->ClearRenderTarget(m_shadowFbo);

		// install shadowmap viewport
		Viewport vp;
		vp.m_x = 0;
		vp.m_y = 0;
		vp.m_width = vp.m_height = kShadowMapSize;
		g_renderDevice->SetViewport(&vp);
	}

	void ShadowsRenderer::RenderMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh)
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

		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, m_znear, m_zfar);

		// calculate view matrix for light
		m_lightViewMatrix = glm::lookAt(glm::vec3(-2.0f, 2.0f, -1.0f),
										  glm::vec3( 0.0f, 0.0f,  0.0f), 
										  glm::vec3( 0.0f, 1.0f,  0.0f));

		m_lightViewProjectionMatrix = lightProjection * m_lightViewMatrix;

		RenderContext renderContext;
		renderContext.width = kShadowMapSize;
		renderContext.height = kShadowMapSize;
		renderContext.proj = lightProjection;
		renderContext.view = m_lightViewMatrix;
		renderContext.model = mesh->GetEntity()->GetWorldTranslation();

		if (mesh->IsA<AnimatedMeshComponent>())
			AnimatedModelRenderer::GetInstance()->Render(dynamicCast<AnimatedMeshComponent>(mesh));

		// shader selection
		IShaderProgram* shaderProgram = nullptr;
		if (mesh->IsA<AnimatedMeshComponent>())
			shaderProgram = m_shadowShader_AnimationMesh;
		else
			shaderProgram = m_shadowShader_StaticMesh;

		g_shaderManager->SetShaderProgram(shaderProgram);

		g_renderer->SetDefaultRenderState();

		if (mesh->IsA<AnimatedMeshComponent>())
		{
			std::shared_ptr<ModelBase> model = mesh->LockModel();
			if (!model)
				return;

			AnimatedModel* animatedModel = dynamicCast<AnimatedModel>(model.get());

			for (const auto& submesh : animatedModel->GetAnimatedSubmehes())
			{
				// transpose matrices for D3D11
				//localCtx.model = glm::transpose(localCtx.model);

				g_renderDevice->SetVertexBuffer(submesh->m_vertexBuffer, sizeof(AnimatedVertex), 0);
				g_renderDevice->SetIndexBuffer(submesh->m_indexBuffer, false);

				ShaderConstantManager::GetInstance()->SetGlobalData(mesh, view, renderContext, graphicsWorld);

				g_renderDevice->DrawIndexed(PM_TriangleList, 0, submesh->m_indicesCount, 0);
			}
		}
		else
		{
			std::shared_ptr<ModelBase> model = mesh->LockModel();

			for (const auto& submesh : model->GetSubmehes())
			{
				// create saved render ctx as previous model.
				RenderContext savedCtx = renderContext;

				// create local copy of render context
				RenderContext localCtx = savedCtx;

				// and overwrite model matrix
				localCtx.model = savedCtx.model * submesh->GetTransform();

				// transpose matrices for D3D11
				//localCtx.model = glm::transpose(localCtx.model);

			//	RenderContext::setContext(localCtx);

				g_renderDevice->SetVertexBuffer(submesh->GetVertexBuffer(), sizeof(Vertex), 0);

				ShaderConstantManager::GetInstance()->SetGlobalData(mesh, view, localCtx, graphicsWorld);
				
				g_renderDevice->Draw(PM_TriangleList, 0, submesh->GetVerticesCount());

				// return what have been
				renderContext = savedCtx;
			}
		}
	}

	void ShadowsRenderer::EndRender()
	{
		// set framebuffer
		//g_renderDevice->SetRenderTarget(g_renderer->getSwapChainRenderTarget());
		g_renderer->setSwapChainRenderTarget();

		// restore viewport
		g_renderDevice->SetViewport(&m_originalViewport);
	}

}