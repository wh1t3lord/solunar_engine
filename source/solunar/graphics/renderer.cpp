#include "graphicspch.h"
#include "graphics/renderer.h"
#include "graphics/screenquad.h"
#include "graphics/gfxcommon.h"
#include "graphics/view.h"
#include "graphics/core/texture.h"
#include "graphics/core/device.h"
#include "graphics/core/rendertarget.h"
#include "graphics/rendercontext.h"

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
#include "graphics/ui/rmlsystem.h"

#include "engine/camera.h"
#include "engine/engine.h"
#include "engine/entity/world.h"
#include "engine/entity/entitymanager.h"
#include "engine/entity/entity.h"

namespace engine
{
	Renderer* g_renderer = nullptr;

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

	void Renderer::initFramebuffer(View* view)
	{
		TextureDesc colorTextureDesc;
		memset(&colorTextureDesc, 0, sizeof(colorTextureDesc));
		colorTextureDesc.m_width = view->m_width;
		colorTextureDesc.m_height = view->m_height;
		colorTextureDesc.m_format = ImageFormat::RGBA16F;

		SubresourceDesc colorTextureSubresourceDesc;
		memset(&colorTextureSubresourceDesc, 0, sizeof(colorTextureSubresourceDesc));

		m_screenColorTexture = g_renderDevice->createTexture2D(colorTextureDesc, colorTextureSubresourceDesc);

		TextureDesc depthTextureDesc;
		memset(&depthTextureDesc, 0, sizeof(depthTextureDesc));
		depthTextureDesc.m_width = view->m_width;
		depthTextureDesc.m_height = view->m_height;
		depthTextureDesc.m_format = ImageFormat::DEPTH24;

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
		// set as lit
		setRenderMode(RendererViewMode::Lit);

		RenderContext::init();

		ShaderConstantManager::getInstance()->init();

		ScreenQuad::init();

		// initialize font manager
		//g_fontManager = mem_new<FontManager>();
		//g_fontManager->initialize();

		// initialize material factory
		MaterialInstanceFactory::createInstance();

		//	m_postProcessingRenderer = PostProcessingRenderer::getInstance();
		//	m_postProcessingRenderer->init(view);

		g_debugRender.initialize();

		// initialize ui renderer
		// RmlSystem::getInstance()->init();
		// RmlSystem::getInstance()->createContext(view->getWidth(), view->getHeight());

	//	initFramebuffer(view);

	//	g_postProcessing.init(view);

		ShadowsRenderer::getInstance()->init();
	}

	void Renderer::shutdown()
	{
		ShadowsRenderer::getInstance()->shutdown();

		//g_postProcessing.shutdown();

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

		//m_postProcessingRenderer->shutdown();
		//m_postProcessingRenderer = nullptr;

	//	MaterialInstanceFactory::destroyInstance();

		// shutdown font manager
		//g_fontManager->shutdown();
		//mem_delete(g_fontManager);

		ScreenQuad::shutdown();

	//	ContentManager::getInstance()->releaseGraphicsObjects();

		//ShaderConstantManager::getInstance()->shutdown();

		RenderContext::shutdown();
	}

	void Renderer::beginFrame()
	{
		//OPTICK_EVENT("Renderer::beginFrame");

		clearScreen();
	}

	void Renderer::renderWorld(View* view)
	{
		//OPTICK_EVENT("Renderer::renderWorld");

		//renderShadows(view);
	
		//g_renderDevice->setRenderTarget(m_screenRenderTarget);
		clearScreen();

		// get camera
		Camera* camera = CameraProxy::getInstance();

		World* world = Engine::ms_world;
		if (world)
		{
			EntityManager& entityManager = world->getEntityManager();

			std::vector<Entity*> drawableEntities = entityManager.getEntitiesWithComponent<MeshComponent>();
			for (auto entity : drawableEntities)
			{
				MeshComponent* meshComponent = entity->getComponent<MeshComponent>();
				meshComponent->render();
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

		//////////////////
		// post processing
		//////////////////
		//g_postProcessing.hdrPass(m_screenColorTexture);
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

}