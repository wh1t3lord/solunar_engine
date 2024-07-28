#include "gldrv_pch.h"

#include "core/file/filesystem.h"

#include "engine/engine.h"
#include "engine/entity/world.h"

#include "gldrv/glrenderer.h"
#include "gldrv/gldevice.h"
#include "gldrv/GLShaderProgramManager.h"
#include "gldrv/glstatemanager.h"

#include "graphics/view.h"
#include "graphics/image.h"
#include "graphics/shaderprogram.h"

#include "graphics/core/device.h"

// light stuff
#include "graphics/light.h"
#include "graphics/lightmanager.h"
#include "graphics/graphicsworld.h"
#include "graphics/shadowsrenderer.h"

// material
#include "graphics/materials/materialinstance.h"

// mesh
#include "graphics/mesh.h"
#include "graphics/material.h"
#include "graphics/animatedmodel.h"

#include "main/main.h"

#include "GL/wglext.h"

namespace engine {

	// this is interface for platform indepented context class.
	class GLPlatformContext : public Object
	{
		ImplementObject(GLPlatformContext, Object);
	public:
		GLPlatformContext();
		virtual ~GLPlatformContext();

		virtual bool create(void* windowHandle);
		virtual void destroy();
		virtual void swapBuffers(int swapInterval);
	};

	GLPlatformContext::GLPlatformContext()
	{
	}

	GLPlatformContext::~GLPlatformContext()
	{
	}

	bool GLPlatformContext::create(void* windowHandle)
	{
		return false;
	}

	void GLPlatformContext::destroy()
	{
	}

	void GLPlatformContext::swapBuffers(int swapInterval)
	{
	}

#ifdef WIN32
	// Windows OpenGL context
	class GLPlatformContext_Win32 : public GLPlatformContext
	{
		ImplementObject(GLPlatformContext_Win32, GLPlatformContext);
	public:
		GLPlatformContext_Win32();
		~GLPlatformContext_Win32();

		bool create(void* windowHandle) override;
		void destroy() override;
		void swapBuffers(int swapInterval) override;

	private:
		HDC m_hDC = NULL;
		HGLRC m_openglContext = NULL;

	};

	GLPlatformContext_Win32::GLPlatformContext_Win32()
	{
	}

	GLPlatformContext_Win32::~GLPlatformContext_Win32()
	{
	}

	bool GLPlatformContext_Win32::create(void* windowHandle)
	{
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
			PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
			32,                   // Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,                   // Number of bits for the depthbuffer
			8,                    // Number of bits for the stencilbuffer
			0,                    // Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		m_hDC = GetDC((HWND)windowHandle);
		int pixelFormat = ChoosePixelFormat(m_hDC, &pfd);
		SetPixelFormat(m_hDC, pixelFormat, &pfd);

		HGLRC tempContext = wglCreateContext(m_hDC);
		wglMakeCurrent(m_hDC, tempContext);

		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		assert(wglCreateContextAttribsARB);

		int attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			WGL_CONTEXT_FLAGS_ARB, 0,
			0
		};

		m_openglContext = wglCreateContextAttribsARB(m_hDC, 0, attribs);
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(tempContext);
		wglMakeCurrent(m_hDC, m_openglContext);

		return true;
	}

	void GLPlatformContext_Win32::destroy()
	{
	}

	void GLPlatformContext_Win32::swapBuffers(int swapInterval)
	{
		SwapBuffers(m_hDC);
	}
#else
	// Linux OpenGL context
	class GLPlatformContext_GLX : public GLPlatformContext
	{
		ImplementObject(GLPlatformContext_GLX, GLPlatformContext);
	public:
		GLPlatformContext_GLX();
		~GLPlatformContext_GLX();

		bool create(void* windowHandle) override;
		void destroy() override;
		void swapBuffers(int swapInterval) override;

	private:
		GLXContext m_openglContext = 0;

	};

	GLPlatformContext_GLX::GLPlatformContext_GLX()
	{
	}

	GLPlatformContext_GLX::~GLPlatformContext_GLX()
	{
	}

	bool GLPlatformContext_GLX::create(void* windowHandle)
	{
		return false;
	}

	void GLPlatformContext_GLX::destroy()
	{
	}

	void GLPlatformContext_GLX::swapBuffers(int swapInterval)
	{
	}
#endif // WIN32

#if 0
	// GL_ARB_debug_output
	PFNGLDEBUGMESSAGECONTROLARBPROC glDebugMessageControlARB;
	PFNGLDEBUGMESSAGEINSERTARBPROC glDebugMessageInsertARB;
	PFNGLDEBUGMESSAGECALLBACKARBPROC glDebugMessageCallbackARB;
	PFNGLGETDEBUGMESSAGELOGARBPROC glGetDebugMessageLogARB;

	void APIENTRY R_GLDebugOutput(GLenum source,
		GLenum type,
		unsigned int id,
		GLenum severity,
		GLsizei length,
		const char* message,
		const void* userParam)
	{
		if (type != GL_DEBUG_TYPE_ERROR)
			return;

		if (type == GL_DEBUG_TYPE_ERROR)
			spdlog::error("[gl]: {}type = 0x{}, severity = 0x{}, message = {}",
				(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR ** " : ""),
				type, severity, message);
		else
			spdlog::info("[gl]: {}type = 0x{}, severity = 0x{}, message = {}",
				(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR ** " : ""),
				type, severity, message);

		if (type == GL_DEBUG_TYPE_ERROR && IsDebuggerPresent())
			DebugBreak();
	}

	void initGlDebug()
	{
		glDebugMessageControlARB = (PFNGLDEBUGMESSAGECONTROLARBPROC)glfwGetProcAddress("glDebugMessageControlARB");
		glDebugMessageInsertARB = (PFNGLDEBUGMESSAGEINSERTARBPROC)glfwGetProcAddress("glDebugMessageInsertARB");
		glDebugMessageCallbackARB = (PFNGLDEBUGMESSAGECALLBACKARBPROC)glfwGetProcAddress("glDebugMessageCallbackARB");
		glGetDebugMessageLogARB = (PFNGLGETDEBUGMESSAGELOGARBPROC)glfwGetProcAddress("glGetDebugMessageLogARB");

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glDebugMessageCallbackARB(R_GLDebugOutput, 0);
	}
#endif

	//void createRenderer()
	//{
	//	g_renderer = mem_new<GLRenderer>();
	//}

	//void destroyRenderer()
	//{
	//	if (g_renderer)
	//	{
	//		mem_delete(g_renderer);
	//		g_renderer = nullptr;
	//	}
	//}

	void registerGLRendererClasses()
	{
		const TypeInfo* classes[] =
		{
			ObjectGetTypeInfo(GLPlatformContext),
#ifdef WIN32
			ObjectGetTypeInfo(GLPlatformContext_Win32),
#else
			ObjectGetTypeInfo(GLPlatformContext_GLX),
#endif // WIN32
		};

		for (int i = 0; i < sizeof(classes) / sizeof(classes[0]); i++)
			TypeManager::getInstance()->registerType(classes[i]);
	}

	GLPlatformContext* g_platformContext = nullptr;

	GLRenderer::GLRenderer()
	{
		m_makeScreenshot = false;
		m_rasterizerState = nullptr;
	}

	GLRenderer::~GLRenderer()
	{

	}

	void findExtensions()
	{
		int externsionsCount = 0;
		glGetIntegerv(GL_NUM_EXTENSIONS, &externsionsCount);

		for (int i = 0; i < externsionsCount; i++)
		{
			const char* externsionName = (const char*)glGetStringi(GL_EXTENSIONS, i);

			if (strcmp(externsionName, "GL_ARB_sampler_objects") == 0)
				Core::msg("[gldrv]: found GL_ARB_sampler_objects...");
			if (strcmp(externsionName, "GL_ARB_separate_shader_objects") == 0)
				Core::msg("[gldrv]: found GL_ARB_separate_shader_objects...");
			if (strcmp(externsionName, "GL_EXT_direct_state_access") == 0)
				Core::msg("[gldrv]: found GL_EXT_direct_state_access...");
		}

		//exit(0);
	}

	GLuint g_vertexArrayObject = 0;

	void GLRenderer::init()
	{
		registerGLRendererClasses();

		// Create OpenGL context
#ifdef WIN32
		g_platformContext = (GLPlatformContext*)TypeManager::getInstance()->createObjectByName("GLPlatformContext_Win32");
#else
#error "Please implement context here for you're platform'!"
#endif // WIN32

		// create context itself
		g_platformContext->create(appGetWindow());

		// initalize glad
		if (!gladLoadGL())
			Core::error("Failed to load OpenGL");

		glEnable(GL_CULL_FACE);

		findExtensions();

		int maxVertexUniformBlocks = 0;
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &maxVertexUniformBlocks);
		Core::msg("GLRenderer: Max vertex uniform blocks: %i", maxVertexUniformBlocks);

#if 0
		initGlDebug();
#endif

		glGenVertexArrays(1, &g_vertexArrayObject);
		glBindVertexArray(g_vertexArrayObject);

		// Initialize render device
		g_renderDevice = (IRenderDevice*)mem_new<GLDevice>();

		// initialize device state manager
		g_stateManager = mem_new<GLStateManager>();
		g_glStateManager->init();

		// 
		RasterizerStateDesc rasterizerState;
		memset(&rasterizerState, 0, sizeof(rasterizerState));
		rasterizerState.m_cullMode = CullMode::Back;
		rasterizerState.m_frontCCW = true;
		rasterizerState.m_fillMode = FillMode::Solid;
		rasterizerState.m_scissorEnable = false;

		m_rasterizerState = g_stateManager->createRasterizerState(rasterizerState);
		g_stateManager->setRasterizerState(m_rasterizerState);

		// Initialize shader manager with current api
		g_shaderManager = mem_new<GLShaderProgramManager>();
		g_shaderManager->init("shaders/gl");

		// initialize base renderer after device creation
		Renderer::init();
	}

	void GLRenderer::shutdown()
	{
		Renderer::shutdown();

		if (g_shaderManager)
		{
			mem_delete(g_shaderManager);
			g_shaderManager = nullptr;
		}

		glBindVertexArray(0);
		glDeleteVertexArrays(1, &g_vertexArrayObject);

		g_glStateManager->shutdown();
		mem_delete(g_stateManager);
		g_stateManager = nullptr;

		mem_delete(g_renderDevice);
		g_renderDevice = nullptr;

		g_platformContext->destroy();

		mem_delete(g_platformContext);
		g_platformContext = nullptr;
	}

	void GLRenderer::beginFrame()
	{
		glBindVertexArray(g_vertexArrayObject);

		g_stateManager->setRasterizerState(m_rasterizerState);
		
		Renderer::beginFrame();
	}

	void GLRenderer::endFrame()
	{
		Renderer::endFrame();

		if (m_makeScreenshot)
			takeScreenshotInternal(); // will reset m_makeScreenshot

		g_platformContext->swapBuffers(0);
	}

	void GLRenderer::takeScreenshot()
	{
		m_makeScreenshot = true;

#if 0
		std::string defpath = FileDevice::getInstance()->getDefaultPath();
		FileDevice::getInstance()->setDefaultPath("");

		char buffer[256];
		for (int i = 0;; i++)
		{
			sprintf(buffer, "sshot_%i.png", i);
			if (!fileExist(buffer))
				break;
		}

		m_screenshotFilename = buffer;
#endif
	}

	void GLRenderer::bindMaterialForMesh(MeshComponent* mesh, Material* material, IMaterialInstance* materialInstance)
	{
		// OPTICK_EVENT("GLRenderer::bindMaterialForMesh");

		Assert(mesh);
		Assert(material);
		Assert(materialInstance);

		// bind material samplers
		material->bind();

		// Initialize shader
		uint32_t pixelVariation = 0;
		if (material->m_selfillum)
			pixelVariation |= PixelVariation_Unlit;
		else
			pixelVariation |= PixelVariation_Lit;

		IShaderProgram* shaderProgram = nullptr;

		if (mesh->isA<AnimatedMeshComponent>())
			shaderProgram = materialInstance->getShaderProgramVariation(VertexFactory_SkinnedMesh, pixelVariation);
		else
			shaderProgram = materialInstance->getShaderProgramVariation(VertexFactory_StaticMesh, pixelVariation);

		Assert2(shaderProgram, "Unknowed mesh component type!");

		// bind material instance shader and material uniforms
		g_shaderManager->setShaderProgram(shaderProgram);
	}

	void GLRenderer::renderMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh)
	{
		//if (MeshComponent* staticMesh = dynamicCast<MeshComponent>(mesh))

		// setup lights
		setupLights(graphicsWorld);

		if (mesh->isA<AnimatedMeshComponent>())
			renderAnimatedMesh(graphicsWorld, view, mesh);
		else
			renderStaticMesh(graphicsWorld, view, mesh);
	}

	void GLRenderer::renderStaticMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh)
	{
		// OPTICK_EVENT("GLRenderer::renderStaticMesh");

		std::shared_ptr<ModelBase> model = mesh->lockModel();

		for (const auto& submesh : model->getSubmehes())
		{
			// create saved render ctx as previous model.
			RenderContext savedCtx = RenderContext::getContext();

			// create local copy of render context
			RenderContext localCtx = RenderContext::getContext();

			// and overwrite model matrix
			localCtx.model = savedCtx.model * submesh->getTransform();

			// transpose matrices for D3D11
			//localCtx.model = glm::transpose(localCtx.model);

			// set our local render ctx
			RenderContext::setContext(localCtx);

			g_renderDevice->setVertexBuffer(submesh->getVertexBuffer(), sizeof(Vertex), 0);

			//g_renderDevice->setIndexBuffer(it->getIndexBuffer());

			//it->getMaterial()->bind();

			std::shared_ptr<Material> material = submesh->lockMaterial();
			bindMaterialForMesh(mesh, material.get(), material->getMaterialInstance());

			ShaderConstantManager::getInstance()->setStaticMeshGlobalData(mesh, view, localCtx, graphicsWorld);

			// install polygon fill mode based on which mode set now

			// if we showing polys
			if (m_meshPolysWireframe && m_currentViewMode != RendererViewMode::Wireframe)
			{
				// render mesh normaly
				//glDrawElements(GL_TRIANGLES, it->getIndeciesCount(), GL_UNSIGNED_BYTE, NULL);
				//glDrawArrays(GL_TRIANGLES, 0, it->getVerticesCount());

				// set polygon fill to lines
			//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				// hack view
				RenderContext hackHackHack = localCtx;
				hackHackHack.proj[2][3] -= 0.0001f;
				RenderContext::setContext(hackHackHack);

				// hack the view
				RendererViewMode savedViewMode = m_currentViewMode;
				m_currentViewMode = RendererViewMode::Wireframe;

				// bind material again
				bindMaterialForMesh(mesh, material.get(), material->getMaterialInstance());

				// draw with lines
				g_renderDevice->draw(PM_TriangleList, 0, submesh->getVerticesCount());
				//glDrawArrays(GL_TRIANGLES, 0, it->getVerticesCount());
				//glDrawElements(GL_TRIANGLES, it->getIndeciesCount(), GL_UNSIGNED_BYTE, NULL);

				// reset view mode
				m_currentViewMode = savedViewMode;

				// reset mode
				//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else
			{
				//	if (getRenderMode() == RendererViewMode::Wireframe)
				//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				g_renderDevice->draw(PM_TriangleList, 0, submesh->getVerticesCount());
				//	glDrawArrays(GL_TRIANGLES, 0, it->getVerticesCount());
					//glDrawElements(GL_TRIANGLES, it->getIndeciesCount(), GL_UNSIGNED_BYTE, NULL);

					// reset
				//	if (getRenderMode() == RendererViewMode::Wireframe)
				//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			// return what have been
			RenderContext::setContext(savedCtx);
		}
	}

	void GLRenderer::renderAnimatedMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh)
	{
		// OPTICK_EVENT("GLRenderer::renderAnimatedMesh");

		std::shared_ptr<ModelBase> model = mesh->lockModel();
		AnimatedModel* animatedModel = dynamicCast<AnimatedModel>(model.get());

		for (const auto& submesh : animatedModel->getAnimatedSubmehes())
		{
			// create saved render ctx as previous model.
			RenderContext savedCtx = RenderContext::getContext();

			// create local copy of render context
			RenderContext localCtx = RenderContext::getContext();

			// transpose matrices for D3D11
			//localCtx.model = glm::transpose(localCtx.model);

			// set our local render ctx
			RenderContext::setContext(localCtx);

			g_renderDevice->setVertexBuffer(submesh->m_vertexBuffer, sizeof(AnimatedVertex), 0);
			g_renderDevice->setIndexBuffer(submesh->m_indexBuffer, false);

			//g_renderDevice->setIndexBuffer(it->getIndexBuffer());

			//it->getMaterial()->bind();

			std::shared_ptr<Material> material = submesh->m_material.lock();
			bindMaterialForMesh(mesh, material.get(), material->getMaterialInstance());

			ShaderConstantManager::getInstance()->setStaticMeshGlobalData(mesh, view, localCtx, graphicsWorld);

			// install polygon fill mode based on which mode set now

			// if we showing polys
			if (m_meshPolysWireframe && m_currentViewMode != RendererViewMode::Wireframe)
			{
				// render mesh normaly
				//glDrawElements(GL_TRIANGLES, it->getIndeciesCount(), GL_UNSIGNED_BYTE, NULL);
				//glDrawArrays(GL_TRIANGLES, 0, it->getVerticesCount());

				// set polygon fill to lines
			//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				// hack view
				RenderContext hackHackHack = localCtx;
				hackHackHack.proj[2][3] -= 0.0001f;
				RenderContext::setContext(hackHackHack);

				// hack the view
				RendererViewMode savedViewMode = m_currentViewMode;
				m_currentViewMode = RendererViewMode::Wireframe;

				// bind material again
				bindMaterialForMesh(mesh, material.get(), material->getMaterialInstance());

				// draw with lines
				g_renderDevice->drawIndexed(PM_TriangleList, 0, submesh->m_indicesCount, 0);
				//g_renderDevice->draw(PM_TriangleList, 0, submesh->m_verticesCount);
				//glDrawArrays(GL_TRIANGLES, 0, it->getVerticesCount());
				//glDrawElements(GL_TRIANGLES, it->getIndeciesCount(), GL_UNSIGNED_BYTE, NULL);

				// reset view mode
				m_currentViewMode = savedViewMode;

				// reset mode
				//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else
			{
				//	if (getRenderMode() == RendererViewMode::Wireframe)
				//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				g_renderDevice->drawIndexed(PM_TriangleList, 0, submesh->m_indicesCount, 0);
				//g_renderDevice->draw(PM_TriangleList, 0, submesh->m_verticesCount);
				//	glDrawArrays(GL_TRIANGLES, 0, it->getVerticesCount());
					//glDrawElements(GL_TRIANGLES, it->getIndeciesCount(), GL_UNSIGNED_BYTE, NULL);

					// reset
				//	if (getRenderMode() == RendererViewMode::Wireframe)
				//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			// return what have been
			RenderContext::setContext(savedCtx);
		}
	}

	void GLRenderer::renderStaticMeshCustomShader(View* view, MeshComponent* mesh, IShaderProgram* customShader)
	{
#if 0
		//GraphicsDevice* graphicsDevice = GraphicsDevice::instance();

		for (auto it : mesh->getModel()->getSubmehes())
		{
			// create saved render ctx as previous model.
			RenderContext savedCtx = RenderContext::getContext();

			// create local copy of render context
			RenderContext localCtx = RenderContext::getContext();

			// and overwrite model matrix
			localCtx.model = savedCtx.model * it->getTransform();

			// set our local render ctx
			RenderContext::setContext(localCtx);

			g_renderDevice->setVertexBuffer(it->getVertexBuffer(), sizeof(Vertex), 0);
			g_renderDevice->setVertexFormat(&s_vfVertex);

			it->getMaterial()->bind();

			g_shaderManager->setShaderProgram(customShader);
			it->getMaterial()->bindUniformsCustom(customShader);

			/*graphicsDevice->setIndexBuffer(it->getIndexBuffer());

			graphicsDevice->drawElements(PrimitiveMode::Triangles, it->getIndeciesCount());*/

			glDrawArrays(GL_TRIANGLES, 0, it->getVerticesCount());

			// return what have been
			RenderContext::setContext(savedCtx);
		}
#endif
	}

	void GLRenderer::renderShadows(View* view)
	{
		ShadowsRenderer* shadowRenderer = ShadowsRenderer::getInstance();
		shadowRenderer->beginRender();

		// initialize render context
		RenderContext& renderContext = RenderContext::getContext();
		renderContext.width = view->m_width;
		renderContext.height = view->m_height;
		renderContext.proj = view->m_projection;
		renderContext.view = view->m_view;
		renderContext.model = glm::mat4(1.0f);
		RenderContext::setContext(renderContext);

#if 0
		const std::shared_ptr<GraphicsWorld>& graphicsWorld = WorldManager::getActiveWorld()->getWorldComponent<GraphicsWorld>();
		const auto& pointLights = graphicsWorld->getLightManager()->getLights();
		for (auto it : pointLights)
		{
			if (!it->isA(PointLightComponent::getStaticTypeInfo()))
				continue;

			for (auto it2 : graphicsWorld->getDrawables())
			{
				if (it2->isA(StaticMeshComponent::getStaticTypeInfo()))
					renderStaticMeshCustomShader(view, dynamicCast<StaticMeshComponent>(it2), shadowRenderer->getShadowMapShader());
			}
		}
#endif
		shadowRenderer->endRender();
	}

	enum ContantBuffersBinding
	{
		PointLights = 0,
		MaterialData = 1,
	};

	void GLRenderer::setRenderModeForShader(IShaderProgram* shaderProgram)
	{
		//switch (m_currentViewMode)
		//{
		//case RendererViewMode::Wireframe:
		//	shaderProgram->setInteger("wireframe", 1);
		//	shaderProgram->setInteger("unlit", 0);
		//	break;
		//case RendererViewMode::Unlit:
		//	shaderProgram->setInteger("unlit", 1);
		//	shaderProgram->setInteger("wireframe", 0);
		//	break;
		//case RendererViewMode::Lit:
		//	shaderProgram->setInteger("unlit", 0);
		//	shaderProgram->setInteger("wireframe", 0);
		//	break;
		//}
	}

	void GLRenderer::takeScreenshotInternal()
	{
		int width = m_view->m_width;
		int height = m_view->m_height;

		size_t bufferSize = 3 * width * height;
		uint8_t* screenBuffer = new uint8_t[bufferSize];

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, screenBuffer);

		Image image;
		image.createRaw((void*)screenBuffer, width, height, 4);
		image.setFlip(false);

		char buffer[256];
		for (int i = 0;; i++)
		{
			sprintf(buffer, "sshot_%i.png", i);

			FileHandle fh = g_fileSystem->open(buffer);
			if (!fh)
			{
				break;
			}
			else
			{
				g_fileSystem->close(fh);
			}
		}

		image.save(buffer);

		delete[] screenBuffer;

		m_makeScreenshot = false;
	}

	void GLRenderer::clearScreen()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
	}

	void GLRenderer::clearRenderTarget(IRenderTarget* renderTarget)
	{
	}

	void GLRenderer::setSwapChainRenderTarget()
	{
	}

}
