#include "graphicspch.h"
#include "graphics/renderer.h"
#include "graphics/screenquad.h"
#include "graphics/gfxcommon.h"
#include "graphics/view.h"
#include "graphics/core/texture.h"
#include "graphics/core/device.h"
#include "graphics/core/rendertarget.h"
#include "graphics/rendercontext.h"
#include "graphics/core/statemanager.h"

// shaders
#include "graphics/ShaderProgramManager.h"

// materials
#include "graphics/materials/materialinstancefactory.h"

// render stuff
#include "graphics/shaderconstantmanager.h"
#include "graphics/shadowsrenderer.h"
#include "graphics/image.h"
#include "graphics/graphicsworld.h"
#include "graphics/lightmanager.h"
#include "graphics/debugrenderer.h"
#include "graphics/ifontmanager.h"
#include "graphics/fontmanager.h"
#include "graphics/mesh.h"
#include "graphics/light.h"
#include "graphics/ui/rmlsystem.h"
#include "graphics/postfxmanager.h"
#include "graphics/imguimanager.h"
#include "graphics/animatedmodel.h"

#include "engine/camera.h"
#include "engine/engine.h"
#include "engine/entity/world.h"
#include "engine/entity/entitymanager.h"
#include "engine/entity/entity.h"

namespace solunar
{
	Renderer* g_renderer = nullptr;

	static std::weak_ptr<TextureMap> g_defaultTexture;
	static std::weak_ptr<TextureMap> g_notFoundTexture;
	static std::weak_ptr<Material> g_defaultMaterial;

	ISamplerState* g_defaultSampler = nullptr;
	IDepthStencilState* g_depthStencilState_NoWrite = nullptr;

	Renderer::Renderer()
	{
		m_meshPolysWireframe	= false;
		m_showOctree			= false;
		m_screenRenderTarget	= nullptr;
		m_screenDepthTexture	= nullptr;
		m_screenColorTexture	= nullptr;
	}

	Renderer::~Renderer()
	{

	}

	void Renderer::initInplaceResources()
	{
#if 0
		// check for shaders
		// #TODO: rewrite to content manager (should return nullptr on stream open failure)
		if (!g_fileSystem->Exist("data/shaders/dx11/quad.vsh") ||
			!g_fileSystem->Exist("data/shaders/dx11/quad.psh"))
		{
			Core::Error("Game files is corrupted or incomplete.\nFor devs: wrong working directory???");
		}
#endif

		SamplerDesc samplerDesc = {};
		samplerDesc.m_wrapS = TextureWrap::Repeat;
		samplerDesc.m_wrapT = TextureWrap::Repeat;
		samplerDesc.m_minFilter = TextureFilter::Nearest;
		samplerDesc.m_magFilter = TextureFilter::Nearest;
		g_defaultSampler = g_renderDevice->CreateSamplerState(samplerDesc);

		DepthStencilDesc desc = {};
		desc.m_depthEnable = false;
		desc.m_depthWriteMask = DEPTH_WRITE_MASK_ALL;
		desc.m_depthFunc = COMPARISON_ALWAYS;
		desc.m_stencilEnable = false;
		desc.m_frontFace.m_stencilFailOp = desc.m_frontFace.m_stencilDepthFailOp = desc.m_frontFace.m_stencilPassOp = STENCIL_OP_KEEP;
		desc.m_frontFace.m_stencilFunc = COMPARISON_ALWAYS;
		desc.m_backFace = desc.m_frontFace;
		g_depthStencilState_NoWrite = g_stateManager->createDepthStencilState(desc);

		g_defaultMaterial = g_contentManager->LoadObject<Material>("materials/default_material.xml");

		// Load textures
		//g_defaultTexture = g_contentManager->LoadObject<TextureMap>("textures/default.png");
		//g_notFoundTexture = g_contentManager->LoadObject<TextureMap>("textures/notexture.png");
	}

	void Renderer::initFramebuffer(View* view)
	{
		TextureDesc colorTextureDesc;
		memset(&colorTextureDesc, 0, sizeof(colorTextureDesc));
		colorTextureDesc.m_width = view->m_width;
		colorTextureDesc.m_height = view->m_height;
		colorTextureDesc.m_format = ImageFormat::RGBA16F;
		colorTextureDesc.m_renderTargetUsage = true;

		SubresourceDesc colorTextureSubresourceDesc;
		memset(&colorTextureSubresourceDesc, 0, sizeof(colorTextureSubresourceDesc));

		m_screenColorTexture = g_renderDevice->CreateTexture2D(colorTextureDesc, colorTextureSubresourceDesc);
		m_screenColorTexture->setDebugName("Screen Color Texture");

		TextureDesc depthTextureDesc;
		memset(&depthTextureDesc, 0, sizeof(depthTextureDesc));
		depthTextureDesc.m_width = view->m_width;
		depthTextureDesc.m_height = view->m_height;
		depthTextureDesc.m_format = ImageFormat::DEPTH24;
		depthTextureDesc.m_renderTargetUsage = true;

		SubresourceDesc depthTextureSubresourceDesc;
		memset(&depthTextureSubresourceDesc, 0, sizeof(depthTextureSubresourceDesc));

		m_screenDepthTexture = g_renderDevice->CreateTexture2D(depthTextureDesc, depthTextureSubresourceDesc);
		m_screenDepthTexture->setDebugName("Screen Depth Texture");

		RenderTargetCreationDesc renderTargetDesc;
		memset(&renderTargetDesc, 0, sizeof(renderTargetDesc));
		renderTargetDesc.m_depthTexture2D = m_screenDepthTexture;
		renderTargetDesc.m_textures2D[0] = m_screenColorTexture;
		renderTargetDesc.m_textures2DCount = 1;
		m_screenRenderTarget = g_renderDevice->CreateRenderTarget(renderTargetDesc);
		m_screenRenderTarget->setDebugName("Screen Render Target");
	}

	void Renderer::Init()
	{
		View* view = CameraProxy::GetInstance()->GetView();

		// Initialize default framebuffer
		initFramebuffer(view);

		// shader manager is inited
		initInplaceResources();

		ImGuiManager::GetInstance()->Init();

		// set as lit
		setRenderMode(RendererViewMode::Lit);

		RenderContext::Init();

		ShaderConstantManager::GetInstance()->Init();

		ScreenQuad::Init();

		// Initialize font manager
		g_fontManager = mem_new<FontManager>();
		g_fontManager->Initialize();

		// Initialize material factory
		MaterialInstanceFactory::CreateInstance();

		g_debugRender.Initialize();

		// Initialize ui renderer
		// RmlSystem::GetInstance()->Init();
		// RmlSystem::GetInstance()->createContext(view->getWidth(), view->getHeight());

		//initFramebuffer(CameraProxy::GetInstance()->GetView());

		g_postFxManager.Init(CameraProxy::GetInstance()->GetView());

		AnimatedModelRenderer::GetInstance()->Init();

//		ShadowsRenderer::GetInstance()->Init();
	}

	void Renderer::Shutdown()
	{
		// Release inplace resources

		ShadowsRenderer::GetInstance()->Shutdown();

		AnimatedModelRenderer::GetInstance()->Shutdown();

		g_postFxManager.Shutdown();

		g_debugRender.Shutdown();

//		RmlSystem::GetInstance()->Shutdown();

		if (m_screenRenderTarget)
		{
			mem_delete(m_screenRenderTarget);
			m_screenRenderTarget = nullptr;
		}

		if (m_screenDepthTexture)
		{
			mem_delete(m_screenDepthTexture);
			m_screenDepthTexture = nullptr;
		}

		if (m_screenColorTexture)
		{
			mem_delete(m_screenColorTexture);
			m_screenColorTexture = nullptr;
		}

		//D3D11: DeviceChild reference counter underflow.Release should not be called on objects with zero reference count.
		//Exception thrown at 0x00007FFE0F6EAB89 (KernelBase.dll) in solunar_main_d.exe : 0x0000087D (parameters : 0x0000000000000000, 0x000000F2E33CD930, 0x000000F2E33CD950).
		//Unhandled exception at 0x00007FFE0F6EAB89 (KernelBase.dll) in solunar_main_d.exe : 0x0000087D (parameters : 0x0000000000000000, 0x000000F2E33CD930, 0x000000F2E33CD950).

		//g_stateManager->destroyDepthStencilState(g_depthStencilState_NoWrite);
		//g_depthStencilState_NoWrite = nullptr;

		mem_delete(g_defaultSampler);
		g_defaultSampler = nullptr;

		//m_postProcessingRenderer->Shutdown();
		//m_postProcessingRenderer = nullptr;

		MaterialInstanceFactory::DestroyInstance();

		// shutdown font manager
		g_fontManager->Shutdown();
		mem_delete(g_fontManager);

		ScreenQuad::Shutdown();

	//	ContentManager::GetInstance()->ReleaseGraphicsObjects();

		ShaderConstantManager::GetInstance()->Shutdown();

		RenderContext::Shutdown();

		ImGuiManager::GetInstance()->Shutdown();
	}

	void Renderer::BeginFrame()
	{
		//OPTICK_EVENT("Renderer::BeginFrame");

		setSwapChainRenderTarget();

		clearScreen();
	}

	void Renderer::RenderWorld(View* view)
	{
		//OPTICK_EVENT("Renderer::RenderWorld");

		//renderShadows(view);
	
		g_renderDevice->SetRenderTarget(m_screenRenderTarget);
		clearRenderTarget(m_screenRenderTarget);

		// get camera
		Camera* camera = CameraProxy::GetInstance();

		World* world = Engine::ms_world;
		if (world)
		{
			EntityManager& entityManager = world->GetEntityManager();

			std::vector<Entity*> drawableEntities = entityManager.GetEntitiesWithComponent<MeshComponent>();

			// color pass
			for (auto entity : drawableEntities)
			{
				MeshComponent* meshComponent = entity->GetComponent<MeshComponent>();
				if (meshComponent)
				{
					// setup render context
					RenderContext& renderCtx = RenderContext::GetContext();
					renderCtx.model = entity->GetWorldTranslation();
					RenderContext::SetContext(renderCtx);

					if (meshComponent->IsA(AnimatedMeshComponent::GetStaticTypeInfo()))
						AnimatedModelRenderer::GetInstance()->Render((AnimatedMeshComponent*)meshComponent);

					// call render function
					renderMesh(world->GetGraphicsWorld(), view, meshComponent);

					meshComponent->Render();
				}
			}
		}

		g_debugRender.RenderFrame(view);
	}

	void Renderer::RenderView(View* view)
	{
		// OPTICK_EVENT("Renderer::RenderView");

		// make context current
	/*	glfwMakeContextCurrent(GraphicsDevice::GetInstance()->getWindow());*/

		// set viewport
		Viewport vp = {};
		vp.m_width = view->m_width;
		vp.m_height = view->m_height;
		g_renderDevice->SetViewport(&vp);

		// Initialize render context
		RenderContext renderContext = RenderContext::GetContext();
		renderContext.width = view->m_width;
		renderContext.height = view->m_height;
		renderContext.proj = view->m_projection;
		renderContext.view = view->m_view;
		renderContext.model = glm::mat4(1.0f);

		RenderContext::SetContext(renderContext);

		RenderWorld(view);

		// post processing
		g_postFxManager.HDRPass(m_screenColorTexture);

		// reset to swap chain
		setSwapChainRenderTarget();

		g_stateManager->SetDepthStencilState(g_depthStencilState_NoWrite, 0);

		// draw
//		ScreenQuad::render( m_screenColorTexture );

		g_fontManager->FlushPrimitives();

		// draw debug renderer
	//	g_debugRender.RenderFrame(view);
	}

	void Renderer::RenderSky(View* view, SkyMeshComponent* skyMesh)
	{
#if 0
		RenderContext& renderCtx = RenderContext::GetContext();
		//renderCtx.model = CameraProxy::GetInstance()->GetCameraComponent();
		RenderContext::SetContext(renderCtx);
		skyMesh->Render();
#endif
	}

	void SetupLightData()
	{
		LightGlobalDataCB* globalData = (LightGlobalDataCB*)g_lightDataConstantBuffer->Map(BufferMapping::WriteOnly);
		globalData->m_pointLightCount = 0;
		globalData->m_spotLightCount = 0;

		g_lightDataConstantBuffer->Unmap();

		g_renderDevice->SetConstantBufferIndex(ConstantBufferBinding_LightData, g_lightDataConstantBuffer.get());
	}

	void SetupDirectionalLight(DirectionalLightComponent* directionalLight)
	{
		if (!directionalLight)
			return;

		DirectionalLightCB* data = (DirectionalLightCB*)g_directionalLightConstantBuffer->Map(BufferMapping::WriteOnly);
		data->m_ambientColor = glm::vec4(directionalLight->m_ambientColor, 1.0f);
		data->m_color = glm::vec4(directionalLight->m_color, 1.0f);

		Assert(directionalLight->GetEntity());

		// convert to euler and reorder direction of the light
#if 0
		glm::quat quaternion = directionalLight->GetEntity()->GetRotation();
		glm::vec3 euler = glm::eulerAngles(quaternion) * 3.14159f / 180.f;
		data->m_direction = glm::vec4(euler.y, euler.x, euler.z, 1.0f);
#else
		glm::quat o = directionalLight->GetEntity()->GetRotation();
		glm::vec3 V;
		V[0] = 2 * (o.x * o.z - o.w * o.y);
		V[1] = 2 * (o.y * o.z + o.w * o.x);
		V[2] = 1 - 2 * (o.x * o.x + o.y * o.y);
		data->m_direction = glm::vec4(-V, 1.0f);

#ifndef FINAL_BUILD
		g_debugRender.DrawLine(directionalLight->GetEntity()->GetPosition(),
			directionalLight->GetEntity()->GetPosition() + V, glm::vec3(1.0f, 1.0f, 0.0f));
#endif // !FINAL_BUILD

#endif
		//	data->m_direction = glm::vec4(glm::radians(directionalLight->m_direction), 1.0f);

		g_directionalLightConstantBuffer->Unmap();

		g_renderDevice->SetConstantBufferIndex(CBBindings_DirectionalLight, g_directionalLightConstantBuffer.get());
	}

	void UpdatePointLightCB(GraphicsWorld* graphicsWorld)
	{
		LightManager* lightMgr = graphicsWorld->GetLightManager();
		
		const std::vector<LightComponent*>& lights = lightMgr->GetLights();
		const std::vector<PointLightComponent*>& pointLights = lightMgr->GetPointLights();

		if (!pointLights.empty())
		{
			PointLightCB* pointLightData = (PointLightCB*)g_pointLightConstantBuffer->Map(BufferMapping::WriteOnly);
			for (int i = 0; i < pointLights.size(); i++)
			{
				auto it = pointLights[i];
				pointLightData->pointLights[i].color = glm::vec4(it->m_color, 1.0f);
				pointLightData->pointLights[i].position = glm::vec4(it->GetEntity()->GetPosition(), 1.0f);
				pointLightData->pointLights[i].specular = glm::vec4(it->m_specularColor, 1.0f);
				pointLightData->pointLights[i].lightData.r = it->m_radius;
			}

			g_pointLightConstantBuffer->Unmap();
			pointLightData = nullptr;
		}

		LightGlobalDataCB* lightGlobalData = (LightGlobalDataCB*)g_lightDataConstantBuffer->Map(BufferMapping::WriteOnly);
		lightGlobalData->m_pointLightCount = (uint32_t)pointLights.size();
		g_lightDataConstantBuffer->Unmap();
		lightGlobalData = nullptr;

		g_renderDevice->SetConstantBufferIndex(ConstantBufferBindings_PointLights, g_pointLightConstantBuffer.get());
		g_renderDevice->SetConstantBufferIndex(ConstantBufferBinding_LightData, g_lightDataConstantBuffer.get());
	}

	void Renderer::SetupLights(GraphicsWorld* graphicsWorld)
	{
		LightManager* lightMgr = graphicsWorld->GetLightManager();
		if (!lightMgr)
			return;

		SetupLightData();

		SetupDirectionalLight(lightMgr->GetDirectionalLight());
		UpdatePointLightCB(graphicsWorld);
	}

	void Renderer::EndFrame()
	{
//		OPTICK_EVENT("Renderer::EndFrame");

		// resetDeviceState();
	}

	void Renderer::ToggleShowingWireframe()
	{
		if (m_currentViewMode == RendererViewMode::Wireframe) {
			m_meshPolysWireframe = false;
		} else {
			m_meshPolysWireframe = !m_meshPolysWireframe;
		}
	}

	void Renderer::ToggleShowOctree()
	{
		m_showOctree = !m_showOctree;
	}

	void Renderer::RenderLoadscreen()
	{
		BeginFrame();
		ImGui::Text("Loading ...");
		EndFrame();
	}

	std::weak_ptr<Material> GetDefaultMaterial()
	{
		return g_defaultMaterial;
	}

}