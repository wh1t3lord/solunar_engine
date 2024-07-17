#include "gldrv_pch.h"

#include "core/file/filesystem.h"

#include "engine/engine.h"
#include "engine/entity/world.h"

#include "gldrv/glrenderer.h"
#include "gldrv/gldevice.h"
#include "gldrv/GLShaderProgramManager.h"
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

namespace engine {

	// #TODO: !!! REMOVE !!!
	// #TODO: !!! UGLY HACK !!!
//	extern GLFWwindow* g_engineWindow;

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

	GLRenderer::GLRenderer()
	{
		m_makeScreenshot = false;
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
		glEnable(GL_CULL_FACE);

		findExtensions();

#if 0
		initGlDebug();
#endif

		glGenVertexArrays(1, &g_vertexArrayObject);
		glBindVertexArray(g_vertexArrayObject);

		// Initialize render device
		g_renderDevice = (IRenderDevice*)mem_new<GLDevice>();

		// initialize base renderer after device creation
		Renderer::init();

		// Initialize shader manager with current api
		g_shaderManager = mem_new<GLShaderProgramManager>();
		g_shaderManager->init("shaders/gl");
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

		mem_delete(g_renderDevice);
		g_renderDevice = nullptr;
	}

	void GLRenderer::beginFrame()
	{
		glBindVertexArray(g_vertexArrayObject);

		Renderer::beginFrame();
	}

	void GLRenderer::endFrame()
	{
		Renderer::endFrame();

		if (m_makeScreenshot)
			takeScreenshotInternal(); // will reset m_makeScreenshot

#if 0
		glfwSwapBuffers(g_engineWindow);
#endif
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
		Assert(mesh);
		Assert(material);
		Assert(materialInstance);

		// bind material samplers
		material->bind();
#if 0
		// Initialize shader
		IShaderProgram* shaderProgram = nullptr;

		if (mesh->isA<StaticMeshComponent>())
			shaderProgram = materialInstance->getStaticMeshShaderProgram();

		Assert2(shaderProgram, "Unknowed mesh component type!");

		// bind material instance shader and material uniforms
		g_shaderManager->setShaderProgram(shaderProgram);
		setRenderModeForShader(shaderProgram);

		if (getRenderMode() == RendererViewMode::Wireframe)
		{
			glm::vec4 wireframeColor;

			if (mesh->isA(StaticMeshComponent::getStaticTypeInfo()))
				wireframeColor = glm::vec4(0.0f, 124.0f / 255.0f, 124.0f / 255.0f, 1.0);

			//shaderProgram->setVector4("wireframeColor", wireframeColor);
		}

		material->bindUniformsCustom(shaderProgram);

		// bind point lights
	//	if (StaticMeshComponent* staticMesh = dynamicCast<StaticMeshComponent>(mesh))
	//		ShaderConstantManager::getInstance()->setStaticMeshGlobalData(staticMesh, view, )
		
		//shaderProgram->setInteger("u_lightsCount", mesh->m_world->getWorldComponent<GraphicsWorld>()->getLightManager()->getLights().size());
		//ShaderConstantManager::getInstance()->setPointLightConstantBuffer();
#endif
	}

	void GLRenderer::renderMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh)
	{
		//if (MeshComponent* staticMesh = dynamicCast<MeshComponent>(mesh))
			renderStaticMesh(graphicsWorld, view, mesh);
	}

	void GLRenderer::renderStaticMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh)
	{
#if 0
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

			//g_renderDevice->setIndexBuffer(it->getIndexBuffer());

			//it->getMaterial()->bind();

			bindMaterialForMesh(mesh, it->getMaterial().get(), it->getMaterial()->getMaterialInstance());

			ShaderConstantManager::getInstance()->setStaticMeshGlobalData(mesh, view, localCtx, graphicsWorld);

			/*graphicsDevice->setIndexBuffer(it->getIndexBuffer());*/

			/*graphicsDevice->drawElements(PrimitiveMode::Triangles, it->getIndeciesCount());*/

			// install polygon fill mode based on which mode set now

			// if we showing polys
			if (m_meshPolysWireframe && m_currentViewMode != RendererViewMode::Wireframe)
			{
				// render mesh normaly
				//glDrawElements(GL_TRIANGLES, it->getIndeciesCount(), GL_UNSIGNED_BYTE, NULL);
				glDrawArrays(GL_TRIANGLES, 0, it->getVerticesCount());

				// set polygon fill to lines
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				// hack view
				RenderContext hackHackHack = localCtx;
				hackHackHack.proj[2][3] -= 0.0001f;
				RenderContext::setContext(hackHackHack);

				// hack the view
				RendererViewMode savedViewMode = m_currentViewMode;
				m_currentViewMode = RendererViewMode::Wireframe;

				// bind material again
				bindMaterialForMesh(mesh, it->getMaterial().get(), it->getMaterial()->getMaterialInstance());

				// draw with lines
				glDrawArrays(GL_TRIANGLES, 0, it->getVerticesCount());
				//glDrawElements(GL_TRIANGLES, it->getIndeciesCount(), GL_UNSIGNED_BYTE, NULL);

				// reset view mode
				m_currentViewMode = savedViewMode;

				// reset mode
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else
			{
				if (getRenderMode() == RendererViewMode::Wireframe)
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				glDrawArrays(GL_TRIANGLES, 0, it->getVerticesCount());
				//glDrawElements(GL_TRIANGLES, it->getIndeciesCount(), GL_UNSIGNED_BYTE, NULL);

				// reset
				if (getRenderMode() == RendererViewMode::Wireframe)
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			// return what have been
			RenderContext::setContext(savedCtx);
		}
#endif
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
	}

}
