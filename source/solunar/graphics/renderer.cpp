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

namespace engine
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
		if (!g_fileSystem->exist("data/shaders/dx11/quad.vsh") ||
			!g_fileSystem->exist("data/shaders/dx11/quad.psh"))
		{
			Core::error("Game files is corrupted or incomplete.\nFor devs: wrong working directory???");
		}
#endif

		SamplerDesc samplerDesc = {};
		samplerDesc.m_wrapS = TextureWrap::Repeat;
		samplerDesc.m_wrapT = TextureWrap::Repeat;
		samplerDesc.m_minFilter = TextureFilter::Nearest;
		samplerDesc.m_magFilter = TextureFilter::Nearest;
		g_defaultSampler = g_renderDevice->createSamplerState(samplerDesc);

		DepthStencilDesc desc = {};
		desc.m_depthEnable = false;
		desc.m_depthWriteMask = DEPTH_WRITE_MASK_ALL;
		desc.m_depthFunc = COMPARISON_ALWAYS;
		desc.m_stencilEnable = false;
		desc.m_frontFace.m_stencilFailOp = desc.m_frontFace.m_stencilDepthFailOp = desc.m_frontFace.m_stencilPassOp = STENCIL_OP_KEEP;
		desc.m_frontFace.m_stencilFunc = COMPARISON_ALWAYS;
		desc.m_backFace = desc.m_frontFace;
		g_depthStencilState_NoWrite = g_stateManager->createDepthStencilState(desc);

		g_defaultMaterial = g_contentManager->loadObject<Material>("materials/default_material.xml");

		// load textures
		//g_defaultTexture = g_contentManager->loadObject<TextureMap>("textures/default.png");
		//g_notFoundTexture = g_contentManager->loadObject<TextureMap>("textures/notexture.png");
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

		m_screenColorTexture = g_renderDevice->createTexture2D(colorTextureDesc, colorTextureSubresourceDesc);

		TextureDesc depthTextureDesc;
		memset(&depthTextureDesc, 0, sizeof(depthTextureDesc));
		depthTextureDesc.m_width = view->m_width;
		depthTextureDesc.m_height = view->m_height;
		depthTextureDesc.m_format = ImageFormat::DEPTH24;
		depthTextureDesc.m_renderTargetUsage = true;

		SubresourceDesc depthTextureSubresourceDesc;
		memset(&depthTextureSubresourceDesc, 0, sizeof(depthTextureSubresourceDesc));

		m_screenDepthTexture = g_renderDevice->createTexture2D(depthTextureDesc, depthTextureSubresourceDesc);

		RenderTargetCreationDesc renderTargetDesc;
		memset(&renderTargetDesc, 0, sizeof(renderTargetDesc));
		renderTargetDesc.m_depthTexture2D = m_screenDepthTexture;
		renderTargetDesc.m_textures2D[0] = m_screenColorTexture;
		renderTargetDesc.m_textures2DCount = 1;
		m_screenRenderTarget = g_renderDevice->createRenderTarget(renderTargetDesc);
	}

	void Renderer::init()
	{
		View* view = CameraProxy::getInstance()->getView();

		// initialize default framebuffer
		initFramebuffer(view);

		// shader manager is inited
		initInplaceResources();

		ImGuiManager::getInstance()->init();

		// set as lit
		setRenderMode(RendererViewMode::Lit);

		RenderContext::init();

		ShaderConstantManager::getInstance()->init();

		ScreenQuad::init();

		// initialize font manager
		g_fontManager = mem_new<FontManager>();
		g_fontManager->initialize();

		// initialize material factory
		MaterialInstanceFactory::createInstance();

		g_debugRender.initialize();

		// initialize ui renderer
		// RmlSystem::getInstance()->init();
		// RmlSystem::getInstance()->createContext(view->getWidth(), view->getHeight());

		//initFramebuffer(CameraProxy::getInstance()->getView());

		g_postFxManager.init(CameraProxy::getInstance()->getView());

		AnimatedModelRenderer::getInstance()->init();

//		ShadowsRenderer::getInstance()->init();
	}

	void Renderer::shutdown()
	{
		// release inplace resources

		ShadowsRenderer::getInstance()->shutdown();

		AnimatedModelRenderer::getInstance()->shutdown();

		g_postFxManager.shutdown();

		g_debugRender.shutdown();

//		RmlSystem::getInstance()->shutdown();

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

		//m_postProcessingRenderer->shutdown();
		//m_postProcessingRenderer = nullptr;

		MaterialInstanceFactory::destroyInstance();

		// shutdown font manager
		g_fontManager->shutdown();
		mem_delete(g_fontManager);

		ScreenQuad::shutdown();

	//	ContentManager::getInstance()->releaseGraphicsObjects();

		ShaderConstantManager::getInstance()->shutdown();

		RenderContext::shutdown();

		ImGuiManager::getInstance()->shutdown();
	}

	void Renderer::beginFrame()
	{
		//OPTICK_EVENT("Renderer::beginFrame");

		setSwapChainRenderTarget();

		clearScreen();
	}

	void Renderer::renderWorld(View* view)
	{
		//OPTICK_EVENT("Renderer::renderWorld");

		//renderShadows(view);
	
		//g_renderDevice->setRenderTarget(m_screenRenderTarget);
		//clearRenderTarget(m_screenRenderTarget);

		// get camera
		Camera* camera = CameraProxy::getInstance();

		World* world = Engine::ms_world;
		if (world)
		{
			EntityManager& entityManager = world->getEntityManager();

			std::vector<Entity*> drawableEntities = entityManager.getEntitiesWithComponent<MeshComponent>();
			
#if 0
			// shadow map pass
			ShadowsRenderer::getInstance()->beginRender();

			// color pass
			for (auto entity : drawableEntities)
			{
				MeshComponent* meshComponent = entity->getComponent<MeshComponent>();
				if (meshComponent)
				{
					// setup render context
					RenderContext& renderCtx = RenderContext::getContext();
					renderCtx.model = entity->getWorldTranslation();
					RenderContext::setContext(renderCtx);

					// call render function
					//renderMesh(world->getGraphicsWorld(), view, meshComponent);
					ShadowsRenderer::getInstance()->renderMesh(world->getGraphicsWorld(), view, meshComponent);

					meshComponent->render();
				}
			}

			ShadowsRenderer::getInstance()->endRender();
#endif
			// color pass
			for (auto entity : drawableEntities)
			{
				MeshComponent* meshComponent = entity->getComponent<MeshComponent>();
				if (meshComponent)
				{
					// setup render context
					RenderContext& renderCtx = RenderContext::getContext();
					renderCtx.model = entity->getWorldTranslation();
					RenderContext::setContext(renderCtx);

					if (meshComponent->isA(AnimatedMeshComponent::getStaticTypeInfo()))
						AnimatedModelRenderer::getInstance()->render((AnimatedMeshComponent*)meshComponent);

					// call render function
					renderMesh(world->getGraphicsWorld(), view, meshComponent);

					meshComponent->render();
				}
			}
		}
#if 0
		const std::shared_ptr<World> world = WorldManager::getActiveWorld();
		if (world)
		{
			std::shared_ptr<GraphicsWorld> graphicsWorld = world->getWorldComponent<GraphicsWorld>();
			if (graphicsWorld)
			{
				// update drawables
				//graphicsWorld->updateDrawables();

				const std::vector<DrawableComponent*>& drawables = graphicsWorld->getDrawables();
				for (const auto it : drawables)
				{
					ASSERT(it);

					if (it->isA(MeshComponent::getStaticTypeInfo()))
					{
						// if we see mesh
						const BoundingBox& aabb = it->getWorldBoundingBox();
						if (!camera->getFrustum().isBoundingBoxInside(aabb))
							continue;

						// setup render context
						RenderContext& renderCtx = RenderContext::getContext();
						
						TransformComponent* transformComponent = (TransformComponent*)g_componentCacheSystem->getComponent(it->getNode(), TransformComponent::getStaticTypeInfo());
						if (transformComponent)
						{
							renderCtx.model = transformComponent->getWorldTranslation();
						}
						else
						{
							renderCtx.model = glm::mat4(1.0f);
						}

						RenderContext::setContext(renderCtx);
			
						// call render function
						renderMesh(graphicsWorld.get(), view, dynamicCast<MeshComponent>(it));
					}
				}
			}

			if (m_showOctree)
				world->getSpacePartition()->render();
		}

#endif

		g_debugRender.renderFrame(view);

		g_fontManager->flushPrimitives();
	}

	void Renderer::renderView(View* view)
	{
		// OPTICK_EVENT("Renderer::renderView");

		// make context current
	/*	glfwMakeContextCurrent(GraphicsDevice::getInstance()->getWindow());*/

		// set viewport
		Viewport vp = {};
		vp.m_width = view->m_width;
		vp.m_height = view->m_height;
		g_renderDevice->setViewport(&vp);

		// initialize render context
		RenderContext renderContext = RenderContext::getContext();
		renderContext.width = view->m_width;
		renderContext.height = view->m_height;
		renderContext.proj = view->m_projection;
		renderContext.view = view->m_view;
		renderContext.model = glm::mat4(1.0f);

		RenderContext::setContext(renderContext);

		renderWorld(view);

		// post processing
		//g_postFxManager.hdrPass(m_screenColorTexture);

		// reset to swap chain
	//	setSwapChainRenderTarget();

	//	g_stateManager->setDepthStencilState(g_depthStencilState_NoWrite, 0);

		// draw
	//	ScreenQuad::render( m_screenColorTexture );

		// draw debug renderer
	//	g_debugRender.renderFrame(view);
	}

	void Renderer::renderSky(View* view, SkyMeshComponent* skyMesh)
	{
#if 0
		RenderContext& renderCtx = RenderContext::getContext();
		//renderCtx.model = CameraProxy::getInstance()->getCameraComponent();
		RenderContext::setContext(renderCtx);
		skyMesh->render();
#endif
	}

	void setupLightData()
	{
		LightGlobalDataCB* globalData = (LightGlobalDataCB*)g_lightDataConstantBuffer->map(BufferMapping::WriteOnly);
		globalData->m_pointLightCount = 0;
		globalData->m_spotLightCount = 0;

		g_lightDataConstantBuffer->unmap();

		g_renderDevice->setConstantBufferIndex(ConstantBufferBinding_LightData, g_lightDataConstantBuffer.get());
	}

	void setupDirectionalLight(DirectionalLightComponent* directionalLight)
	{
		if (!directionalLight)
			return;

		DirectionalLightCB* data = (DirectionalLightCB*)g_directionalLightConstantBuffer->map(BufferMapping::WriteOnly);
		data->m_ambientColor = glm::vec4(directionalLight->m_ambientColor, 1.0f);
		data->m_color = glm::vec4(directionalLight->m_color, 1.0f);

		Assert(directionalLight->getEntity());

		// convert to euler and reorder direction of the light
#if 0
		glm::quat quaternion = directionalLight->getEntity()->getRotation();
		glm::vec3 euler = glm::eulerAngles(quaternion) * 3.14159f / 180.f;
		data->m_direction = glm::vec4(euler.y, euler.x, euler.z, 1.0f);
#else
		glm::quat o = directionalLight->getEntity()->getRotation();
		glm::vec3 V;
		V[0] = 2 * (o.x * o.z - o.w * o.y);
		V[1] = 2 * (o.y * o.z + o.w * o.x);
		V[2] = 1 - 2 * (o.x * o.x + o.y * o.y);
		data->m_direction = glm::vec4(-V, 1.0f);

#ifndef FINAL_BUILD
		g_debugRender.drawLine(directionalLight->getEntity()->getPosition(),
			directionalLight->getEntity()->getPosition() + V, glm::vec3(1.0f, 1.0f, 0.0f));
#endif // !FINAL_BUILD

#endif
		//	data->m_direction = glm::vec4(glm::radians(directionalLight->m_direction), 1.0f);

		g_directionalLightConstantBuffer->unmap();

		g_renderDevice->setConstantBufferIndex(CBBindings_DirectionalLight, g_directionalLightConstantBuffer.get());
	}

	void setupPointLight(GraphicsWorld* graphicsWorld)
	{
		LightManager* lightMgr = graphicsWorld->getLightManager();
		
		const std::vector<LightComponent*>& lights = lightMgr->getLights();

		std::vector<PointLightComponent*> pointLights;
		for (auto& it : lights)
		{
			if (it->isA(PointLightComponent::getStaticTypeInfo()))
				pointLights.push_back(dynamicCast<PointLightComponent>(it));
		}

		PointLightCB* globalData = (PointLightCB*)g_pointLightConstantBuffer->map(BufferMapping::WriteOnly);
		for (int i = 0; i < pointLights.size(); i++)
		{
			auto it = pointLights[i];
			globalData->pointLights[i].color = glm::vec4(it->m_color, 1.0f);
			globalData->pointLights[i].position = glm::vec4(it->getEntity()->getPosition(), 1.0f);
			globalData->pointLights[i].specular = glm::vec4(it->m_specularColor, 1.0f);
			globalData->pointLights[i].lightData.r = it->m_radius;
		}

		g_pointLightConstantBuffer->unmap();
		globalData = nullptr;

		LightGlobalDataCB* lightGlobalData = (LightGlobalDataCB*)g_lightDataConstantBuffer->map(BufferMapping::WriteOnly);
		lightGlobalData->m_pointLightCount = pointLights.size();
		g_lightDataConstantBuffer->unmap();
		lightGlobalData = nullptr;

		g_renderDevice->setConstantBufferIndex(ConstantBufferBindings_PointLights, g_pointLightConstantBuffer.get());
		g_renderDevice->setConstantBufferIndex(ConstantBufferBinding_LightData, g_lightDataConstantBuffer.get());
	}

	void Renderer::setupLights(GraphicsWorld* graphicsWorld)
	{
		LightManager* lightMgr = graphicsWorld->getLightManager();
		if (!lightMgr)
			return;

		setupLightData();

		setupDirectionalLight(lightMgr->getDirectionalLight());
		setupPointLight(graphicsWorld);
	}

	void Renderer::endFrame()
	{
//		OPTICK_EVENT("Renderer::endFrame");

		// resetDeviceState();
	}

	void Renderer::toggleShowingWireframe()
	{
		if (m_currentViewMode == RendererViewMode::Wireframe) {
			m_meshPolysWireframe = false;
		} else {
			m_meshPolysWireframe = !m_meshPolysWireframe;
		}
	}

	void Renderer::toggleShowOctree()
	{
		m_showOctree = !m_showOctree;
	}

	void Renderer::renderLoadscreen()
	{
		beginFrame();
		ImGui::Text("Loading ...");
		endFrame();
	}

	std::weak_ptr<Material> getDefaultMaterial()
	{
		return g_defaultMaterial;
	}

}