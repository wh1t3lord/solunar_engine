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

namespace solunar {

	// this is interface for platform indepented context class.
	class GLPlatformContext : public Object
	{
		DECLARE_OBJECT(GLPlatformContext);
	public:
		GLPlatformContext();
		virtual ~GLPlatformContext();

		virtual bool Create(void* windowHandle);
		virtual void Destroy();
		virtual void swapBuffers(int swapInterval);
	};

	IMPLEMENT_OBJECT(GLPlatformContext, Object);

	GLPlatformContext::GLPlatformContext()
	{
	}

	GLPlatformContext::~GLPlatformContext()
	{
	}

	bool GLPlatformContext::Create(void* windowHandle)
	{
		return false;
	}

	void GLPlatformContext::Destroy()
	{
	}

	void GLPlatformContext::swapBuffers(int swapInterval)
	{
	}

#ifdef WIN32
	// Windows OpenGL context
	class GLPlatformContext_Win32 : public GLPlatformContext
	{
		DECLARE_OBJECT(GLPlatformContext_Win32);
	public:
		GLPlatformContext_Win32();
		~GLPlatformContext_Win32();

		bool Create(void* windowHandle) override;
		void Destroy() override;
		void swapBuffers(int swapInterval) override;

	private:
		HDC m_hDC = NULL;
		HGLRC m_openglContext = NULL;

	};

	IMPLEMENT_OBJECT(GLPlatformContext_Win32, GLPlatformContext);

	GLPlatformContext_Win32::GLPlatformContext_Win32()
	{
	}

	GLPlatformContext_Win32::~GLPlatformContext_Win32()
	{
	}

	bool GLPlatformContext_Win32::Create(void* windowHandle)
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

	void GLPlatformContext_Win32::Destroy()
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
		DECLARE_OBJECT(GLPlatformContext_GLX);
	public:
		GLPlatformContext_GLX();
		~GLPlatformContext_GLX();

		bool Create(void* windowHandle) override;
		void Destroy() override;
		void swapBuffers(int swapInterval) override;

	private:
		GLXContext m_openglContext = 0;

	};

	IMPLEMENT_OBJECT(GLPlatformContext_GLX, GLPlatformContext);

	GLPlatformContext_GLX::GLPlatformContext_GLX()
	{
	}

	GLPlatformContext_GLX::~GLPlatformContext_GLX()
	{
	}

	bool GLPlatformContext_GLX::Create(void* windowHandle)
	{
		return false;
	}

	void GLPlatformContext_GLX::Destroy()
	{
	}

	void GLPlatformContext_GLX::swapBuffers(int swapInterval)
	{
	}
#endif // WIN32

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
		if (type != GL_DEBUG_TYPE_ERROR_ARB)
			return;

		if (type == GL_DEBUG_TYPE_ERROR_ARB)
			Core::Msg("OpenGL Error: %stype = 0x%i, severity = 0x%i, message = %s",
				(type == GL_DEBUG_TYPE_ERROR_ARB ? "** GL ERROR ** " : ""),
				type, severity, message);
		else
			Core::Msg("OpenGL Error: %stype = 0x%i, severity = 0x%i, message = %s",
				(type == GL_DEBUG_TYPE_ERROR_ARB ? "** GL ERROR ** " : ""),
				type, severity, message);

		if (type == GL_DEBUG_TYPE_ERROR_ARB && IsDebuggerPresent())
			DebugBreak();
	}

	void initGlDebug()
	{
		glDebugMessageControlARB = (PFNGLDEBUGMESSAGECONTROLARBPROC)wglGetProcAddress("glDebugMessageControlARB");
		glDebugMessageInsertARB = (PFNGLDEBUGMESSAGEINSERTARBPROC)wglGetProcAddress("glDebugMessageInsertARB");
		glDebugMessageCallbackARB = (PFNGLDEBUGMESSAGECALLBACKARBPROC)wglGetProcAddress("glDebugMessageCallbackARB");
		glGetDebugMessageLogARB = (PFNGLGETDEBUGMESSAGELOGARBPROC)wglGetProcAddress("glGetDebugMessageLogARB");

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glDebugMessageCallbackARB(R_GLDebugOutput, 0);
	}

	void createRenderer()
	{
		g_renderer = mem_new<GLRenderer>();
	}

	void destroyRenderer()
	{
		if (g_renderer)
		{
			mem_delete(g_renderer);
			g_renderer = nullptr;
		}
	}

	void registerGLRendererClasses()
	{
		const TypeInfo* classes[] =
		{
			OBJECT_GET_TYPEINFO(GLPlatformContext),
#ifdef WIN32
			OBJECT_GET_TYPEINFO(GLPlatformContext_Win32),
#else
			OBJECT_GET_TYPEINFO(GLPlatformContext_GLX),
#endif // WIN32
		};

		for (int i = 0; i < sizeof(classes) / sizeof(classes[0]); i++)
			TypeManager::GetInstance()->RegisterType(classes[i]);
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
				Core::Msg("[gldrv]: found GL_ARB_sampler_objects...");
			if (strcmp(externsionName, "GL_ARB_separate_shader_objects") == 0)
				Core::Msg("[gldrv]: found GL_ARB_separate_shader_objects...");
			if (strcmp(externsionName, "GL_EXT_direct_state_access") == 0)
				Core::Msg("[gldrv]: found GL_EXT_direct_state_access...");
		}

		//exit(0);
	}

	bool is_GL_ARB_debug_output_supported()
	{
		int externsionsCount = 0;
		glGetIntegerv(GL_NUM_EXTENSIONS, &externsionsCount);

		for (int i = 0; i < externsionsCount; i++)
		{
			const char* externsionName = (const char*)glGetStringi(GL_EXTENSIONS, i);

			if (strcmp(externsionName, "GL_ARB_debug_output") == 0)
			{
				Core::Msg("[gldrv]: found GL_ARB_debug_output...");
				return true;
			}
		}

		return false;
	}

	GLuint g_vertexArrayObject = 0;

	void GLRenderer::Init()
	{
		registerGLRendererClasses();

		// Create OpenGL context
#ifdef WIN32
		g_platformContext = (GLPlatformContext*)TypeManager::GetInstance()->CreateObjectByName("GLPlatformContext_Win32");
#else
#error "Please implement context here for you're platform'!"
#endif // WIN32

		// create context itself
		g_platformContext->Create(appGetWindow());

		// initalize glad
		if (!gladLoadGL())
			Core::Error("Failed to load OpenGL");

		// Clear OpenGL Error
		glGetError();

		glEnable(GL_CULL_FACE);

		findExtensions();

		int maxVertexUniformBlocks = 0;
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &maxVertexUniformBlocks);
		Core::Msg("GLRenderer: Max vertex uniform blocks: %i", maxVertexUniformBlocks);

		if (is_GL_ARB_debug_output_supported())
			initGlDebug();

		glGenVertexArrays(1, &g_vertexArrayObject);
		glBindVertexArray(g_vertexArrayObject);

		// Initialize render device
		g_renderDevice = (IRenderDevice*)mem_new<GLDevice>();

		// Initialize device state manager
		g_stateManager = mem_new<GLStateManager>();
		g_glStateManager->Init();

		// 
		RasterizerStateDesc rasterizerState;
		memset(&rasterizerState, 0, sizeof(rasterizerState));
		rasterizerState.m_cullMode = CullMode::Back;
		rasterizerState.m_frontCCW = true;
		rasterizerState.m_fillMode = FillMode::Solid;
		rasterizerState.m_scissorEnable = false;

		m_rasterizerState = g_stateManager->CreateRasterizerState(rasterizerState);
		g_stateManager->SetRasterizerState(m_rasterizerState);

		// Initialize shader manager with current api
		g_shaderManager = mem_new<GLShaderProgramManager>();
		g_shaderManager->Init("shaders/gl");

		// Initialize base renderer after device creation
		Renderer::Init();
	}

	void GLRenderer::Shutdown()
	{
		Renderer::Shutdown();

		if (g_shaderManager)
		{
			mem_delete(g_shaderManager);
			g_shaderManager = nullptr;
		}

		glBindVertexArray(0);
		glDeleteVertexArrays(1, &g_vertexArrayObject);

		g_glStateManager->Shutdown();
		mem_delete(g_stateManager);
		g_stateManager = nullptr;

		mem_delete(g_renderDevice);
		g_renderDevice = nullptr;

		g_platformContext->Destroy();

		mem_delete(g_platformContext);
		g_platformContext = nullptr;
	}

	void GLRenderer::BeginFrame()
	{
		glBindVertexArray(g_vertexArrayObject);

		g_stateManager->SetRasterizerState(m_rasterizerState);
		
		Renderer::BeginFrame();
	}

	void GLRenderer::EndFrame()
	{
		Renderer::EndFrame();

		if (m_makeScreenshot)
			takeScreenshotInternal(); // will reset m_makeScreenshot

		g_platformContext->swapBuffers(0);
	}

	void GLRenderer::TakeScreenshot()
	{
		m_makeScreenshot = true;

#if 0
		std::string defpath = FileDevice::GetInstance()->getDefaultPath();
		FileDevice::GetInstance()->setDefaultPath("");

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

	void GLRenderer::BindMaterialForMesh(MeshComponent* mesh, Material* material, IMaterialInstance* materialInstance)
	{
		// OPTICK_EVENT("GLRenderer::BindMaterialForMesh");

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

		if (mesh->IsA<AnimatedMeshComponent>())
			shaderProgram = materialInstance->getShaderProgramVariation(VertexFactory_SkinnedMesh, pixelVariation);
		else
			shaderProgram = materialInstance->getShaderProgramVariation(VertexFactory_StaticMesh, pixelVariation);

		Assert2(shaderProgram, "Unknowed mesh component type!");

		// bind material instance shader and material uniforms
		g_shaderManager->SetShaderProgram(shaderProgram);
	}

	void GLRenderer::RenderMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh)
	{
		//if (MeshComponent* staticMesh = dynamicCast<MeshComponent>(mesh))

		// setup lights
		SetupLights(graphicsWorld);

		if (mesh->IsA<AnimatedMeshComponent>())
			RenderAnimatedMesh(graphicsWorld, view, mesh);
		else
			RenderStaticMesh(graphicsWorld, view, mesh);
	}

	void GLRenderer::RenderStaticMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh)
	{
		// OPTICK_EVENT("GLRenderer::RenderStaticMesh");

		std::shared_ptr<ModelBase> model = mesh->LockModel();

		for (const auto& submesh : model->GetSubmehes())
		{
			// create saved render ctx as previous model.
			RenderContext savedCtx = RenderContext::GetContext();

			// create local copy of render context
			RenderContext localCtx = RenderContext::GetContext();

			// and overwrite model matrix
			localCtx.model = savedCtx.model * submesh->GetTransform();

			// transpose matrices for D3D11
			//localCtx.model = glm::transpose(localCtx.model);

			// set our local render ctx
			RenderContext::SetContext(localCtx);

			g_renderDevice->SetVertexBuffer(submesh->GetVertexBuffer(), sizeof(Vertex), 0);

			//g_renderDevice->SetIndexBuffer(it->getIndexBuffer());

			//it->getMaterial()->bind();

			std::shared_ptr<Material> material = submesh->LockMaterial();
			BindMaterialForMesh(mesh, material.get(), material->GetMaterialInstance());

			ShaderConstantManager::GetInstance()->SetGlobalData(mesh, view, localCtx, graphicsWorld);

			// install polygon fill mode based on which mode set now

			// if we showing polys
			if (m_meshPolysWireframe && m_currentViewMode != RendererViewMode::Wireframe)
			{
				// render mesh normaly
				//glDrawElements(GL_TRIANGLES, it->getIndeciesCount(), GL_UNSIGNED_BYTE, NULL);
				//glDrawArrays(GL_TRIANGLES, 0, it->GetVerticesCount());

				// set polygon fill to lines
			//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				// hack view
				RenderContext hackHackHack = localCtx;
				hackHackHack.proj[2][3] -= 0.0001f;
				RenderContext::SetContext(hackHackHack);

				// hack the view
				RendererViewMode savedViewMode = m_currentViewMode;
				m_currentViewMode = RendererViewMode::Wireframe;

				// bind material again
				BindMaterialForMesh(mesh, material.get(), material->GetMaterialInstance());

				// draw with lines
				g_renderDevice->Draw(PM_TriangleList, 0, submesh->GetVerticesCount());
				//glDrawArrays(GL_TRIANGLES, 0, it->GetVerticesCount());
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

				g_renderDevice->Draw(PM_TriangleList, 0, submesh->GetVerticesCount());

				//	glDrawArrays(GL_TRIANGLES, 0, it->GetVerticesCount());
					//glDrawElements(GL_TRIANGLES, it->getIndeciesCount(), GL_UNSIGNED_BYTE, NULL);

					// reset
				//	if (getRenderMode() == RendererViewMode::Wireframe)
				//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			// return what have been
			RenderContext::SetContext(savedCtx);
		}
	}

	void GLRenderer::RenderAnimatedMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh)
	{
		// OPTICK_EVENT("GLRenderer::RenderAnimatedMesh");

		std::shared_ptr<ModelBase> model = mesh->LockModel();
		AnimatedModel* animatedModel = dynamicCast<AnimatedModel>(model.get());

		for (const auto& submesh : animatedModel->GetAnimatedSubmehes())
		{
			// create saved render ctx as previous model.
			RenderContext savedCtx = RenderContext::GetContext();

			// create local copy of render context
			RenderContext localCtx = RenderContext::GetContext();

			// transpose matrices for D3D11
			//localCtx.model = glm::transpose(localCtx.model);

			// set our local render ctx
			RenderContext::SetContext(localCtx);

			g_renderDevice->SetVertexBuffer(submesh->m_vertexBuffer, sizeof(AnimatedVertex), 0);
			g_renderDevice->SetIndexBuffer(submesh->m_indexBuffer, false);

			//g_renderDevice->SetIndexBuffer(it->getIndexBuffer());

			//it->getMaterial()->bind();

			std::shared_ptr<Material> material = submesh->m_material.lock();
			BindMaterialForMesh(mesh, material.get(), material->GetMaterialInstance());

			ShaderConstantManager::GetInstance()->SetGlobalData(mesh, view, localCtx, graphicsWorld);

			// install polygon fill mode based on which mode set now

			// if we showing polys
			if (m_meshPolysWireframe && m_currentViewMode != RendererViewMode::Wireframe)
			{
				// render mesh normaly
				//glDrawElements(GL_TRIANGLES, it->getIndeciesCount(), GL_UNSIGNED_BYTE, NULL);
				//glDrawArrays(GL_TRIANGLES, 0, it->GetVerticesCount());

				// set polygon fill to lines
			//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				// hack view
				RenderContext hackHackHack = localCtx;
				hackHackHack.proj[2][3] -= 0.0001f;
				RenderContext::SetContext(hackHackHack);

				// hack the view
				RendererViewMode savedViewMode = m_currentViewMode;
				m_currentViewMode = RendererViewMode::Wireframe;

				// bind material again
				BindMaterialForMesh(mesh, material.get(), material->GetMaterialInstance());

				// draw with lines
				g_renderDevice->DrawIndexed(PM_TriangleList, 0, submesh->m_indicesCount, 0);
				//g_renderDevice->Draw(PM_TriangleList, 0, submesh->m_verticesCount);
				//glDrawArrays(GL_TRIANGLES, 0, it->GetVerticesCount());
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

				g_renderDevice->DrawIndexed(PM_TriangleList, 0, submesh->m_indicesCount, 0);

				//g_renderDevice->Draw(PM_TriangleList, 0, submesh->m_verticesCount);
				//	glDrawArrays(GL_TRIANGLES, 0, it->GetVerticesCount());
					//glDrawElements(GL_TRIANGLES, it->getIndeciesCount(), GL_UNSIGNED_BYTE, NULL);

					// reset
				//	if (getRenderMode() == RendererViewMode::Wireframe)
				//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			// return what have been
			RenderContext::SetContext(savedCtx);
		}
	}

	void GLRenderer::renderStaticMeshCustomShader(View* view, MeshComponent* mesh, IShaderProgram* customShader)
	{
#if 0
		//GraphicsDevice* graphicsDevice = GraphicsDevice::instance();

		for (auto it : mesh->getModel()->GetSubmehes())
		{
			// create saved render ctx as previous model.
			RenderContext savedCtx = RenderContext::GetContext();

			// create local copy of render context
			RenderContext localCtx = RenderContext::GetContext();

			// and overwrite model matrix
			localCtx.model = savedCtx.model * it->GetTransform();

			// set our local render ctx
			RenderContext::SetContext(localCtx);

			g_renderDevice->SetVertexBuffer(it->GetVertexBuffer(), sizeof(Vertex), 0);
			g_renderDevice->setVertexFormat(&s_vfVertex);

			it->getMaterial()->bind();

			g_shaderManager->SetShaderProgram(customShader);
			it->getMaterial()->bindUniformsCustom(customShader);

			/*graphicsDevice->SetIndexBuffer(it->getIndexBuffer());

			graphicsDevice->drawElements(PrimitiveMode::Triangles, it->getIndeciesCount());*/

			glDrawArrays(GL_TRIANGLES, 0, it->GetVerticesCount());

			// return what have been
			RenderContext::SetContext(savedCtx);
		}
#endif
	}

	void GLRenderer::renderShadows(View* view)
	{
		ShadowsRenderer* shadowRenderer = ShadowsRenderer::GetInstance();
		shadowRenderer->BeginRender();

		// Initialize render context
		RenderContext& renderContext = RenderContext::GetContext();
		renderContext.width = view->m_width;
		renderContext.height = view->m_height;
		renderContext.proj = view->m_projection;
		renderContext.view = view->m_view;
		renderContext.model = glm::mat4(1.0f);
		RenderContext::SetContext(renderContext);

#if 0
		const std::shared_ptr<GraphicsWorld>& graphicsWorld = WorldManager::getActiveWorld()->getWorldComponent<GraphicsWorld>();
		const auto& pointLights = graphicsWorld->GetLightManager()->GetLights();
		for (auto it : pointLights)
		{
			if (!it->IsA(PointLightComponent::GetStaticTypeInfo()))
				continue;

			for (auto it2 : graphicsWorld->getDrawables())
			{
				if (it2->IsA(StaticMeshComponent::GetStaticTypeInfo()))
					renderStaticMeshCustomShader(view, dynamicCast<StaticMeshComponent>(it2), shadowRenderer->getShadowMapShader());
			}
		}
#endif
		shadowRenderer->EndRender();
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

			FileHandle fh = g_fileSystem->Open(buffer);
			if (!fh)
			{
				break;
			}
			else
			{
				g_fileSystem->Close(fh);
			}
		}

		image.Save(buffer);

		delete[] screenBuffer;

		m_makeScreenshot = false;
	}

	void GLRenderer::clearScreen()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
	}

	void GLRenderer::ClearRenderTarget(IRenderTarget* renderTarget)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
	}

	void GLRenderer::setSwapChainRenderTarget()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
