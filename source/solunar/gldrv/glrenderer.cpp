#include "renderer.h"
#include "gldrv_pch.h"
#include "gldrv/glrenderer.h"
#include "gldrv/gldevice.h"
//#include "gldrv/glshaderprogrammanager.h"
//#include "gldrv/glstatemanager.h"
#include "gldrv/glrendertarget.h"

// filesystem
#include "core/file/filesystem.h"

// Base graphics classes
#include "graphics/view.h"
#include "graphics/rendercontext.h"

// Graphics objects
#include "graphics/image.h"
#include "graphics/mesh.h"
//#include "graphics/animatedmodel.h" #TODO: Compile error
#include "graphics/material.h"
#include "graphics/materials/materialinstance.h"
#include "graphics/materials/materialinstance_generic.h"

// Graphics managers
#include "graphics/ShaderProgramManager.h"
#include "graphics/shaderconstantmanager.h"

#include "main/main.h"


namespace engine {

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

	GLRenderer::GLRenderer()
	{
		m_takeScreenshot = false;
	}

	GLRenderer::~GLRenderer()
	{

	}

	void GLRenderer::init()
	{
		glEnable(GL_CULL_FACE);

#if 0
		initGlDebug();
#endif

		// Initialize render device
		g_renderDevice = (IRenderDevice*)mem_new<GLDevice>();

		// initialize base renderer after device creation
		Renderer::init();
	}

	void GLRenderer::shutdown()
	{
		Renderer::shutdown();

		mem_delete(g_renderDevice);
		g_renderDevice = nullptr;
	}

	void GLRenderer::endFrame()
	{
		Renderer::endFrame();

		if (m_takeScreenshot)
			takeScreenshotInternal(); // will reset m_takeScreenshot
	}

	void GLRenderer::takeScreenshot()
	{
		m_takeScreenshot = true;
	}

	void GLRenderer::bindMaterialForMesh(MeshComponent* mesh, Material* material, IMaterialInstance* materialInstance)
	{
		Assert(mesh);
		Assert(material);
		Assert(materialInstance);

#if 0
		IShaderProgram* shaderProgram = nullptr;
		switch (getRenderMode())
		{
		case RendererViewMode::Wireframe:
			shaderProgram = materialInstance->getWireframeShaderProgram();
			break;

		case RendererViewMode::Unlit:
			shaderProgram = materialInstance->getUnlitShaderProgram();
			break;

		case RendererViewMode::Lit:
			shaderProgram = materialInstance->getLitShaderProgram();
			break;

		default:
			ASSERT2(0, "Unkonwed render mode");
			break;
		}
#endif

		// bind material samplers
		material->bind();

		// bind material instance shader and material uniforms
		//ShaderProgramManager::setShaderProgram(shaderProgram);

# if 0
		if (getRenderMode() == RendererViewMode::Wireframe)
		{
			glm::vec4 wireframeColor;

			if (mesh->isA(StaticMeshComponent::getStaticTypeInfo()))
				wireframeColor = glm::vec4(0.0f, 124.0f / 255.0f, 124.0f / 255.0f, 1.0);

			shaderProgram->setVector4("wireframeColor", wireframeColor);
		}

		material->bindUniformsCustom(shaderProgram);


		// bind point lights
		shaderProgram->setInteger("u_lightsCount", mesh->m_world->getWorldComponent<GraphicsWorld>()->getLightManager()->getLights().size());
		ShaderConstantManager::getInstance()->setPointLightConstantBuffer();
# endif
	}

	void GLRenderer::renderMesh(GraphicsWorld* graphicsWorld, View* view, MeshComponent* mesh)
	{
#if 0
		if (mesh->isA(StaticMeshComponent::getStaticTypeInfo()))
		{
			renderStaticMesh(view, dynamicCast<StaticMeshComponent>(mesh));
		}
#endif
	}

	void GLRenderer::renderStaticMesh(View* view, MeshComponent* mesh)
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

			g_renderDevice->setVertexBuffer(it->getVertexBuffer());
			g_renderDevice->setVertexFormat(&s_vfVertex);

			//g_renderDevice->setIndexBuffer(it->getIndexBuffer());

			//it->getMaterial()->bind();

			bindMaterialForMesh(mesh, it->getMaterial().get(), it->getMaterial()->getMaterialInstance());

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

			// reset material bindings
			it->getMaterial()->resetAllStates();
		}
#endif
	}

	void GLRenderer::renderStaticMeshCustomShader(View* view, MeshComponent* mesh, IShaderProgram* customShader)
	{
		//GraphicsDevice* graphicsDevice = GraphicsDevice::instance();
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

			g_renderDevice->setVertexBuffer(it->getVertexBuffer());
			g_renderDevice->setVertexFormat(&s_vfVertex);

			it->getMaterial()->bind();

			ShaderProgramManager::setShaderProgram(customShader);
			it->getMaterial()->bindUniformsCustom(customShader);

			/*graphicsDevice->setIndexBuffer(it->getIndexBuffer());

			graphicsDevice->drawElements(PrimitiveMode::Triangles, it->getIndeciesCount());*/

			glDrawArrays(GL_TRIANGLES, 0, it->getVerticesCount());

			// return what have been
			RenderContext::setContext(savedCtx);

			// reset material bindings
			it->getMaterial()->resetAllStates();
		}
#endif
	}

	void GLRenderer::renderShadows(View* view)
	{
#if 0
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

		shadowRenderer->endRender();
#endif
	}

	enum ContantBuffersBinding
	{
		PointLights = 0,
		MaterialData = 1,
	};

	void GLRenderer::updateGlobalConstants(World* world, IShaderProgram* shader)
	{
		updatePointLightsConstants(world, shader);
	}

	void GLRenderer::updatePointLightsConstants(World* world, IShaderProgram* shader)
	{
#if 0
		// get buffer location
		int pointLightsCBLocation = shader->getUniformBlockLocation("PointLights");

		// assing binding
		glUniformBlockBinding(shader->getShaderHandle(), pointLightsCBLocation, ContantBuffersBinding::PointLights);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// #TODO: TOO HACKY
		// #HACK: TOO HACKY
		// too hacky
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		BufferBase* pointLightCB = ShaderConstantManager::getInstance()->getPointLightsConstantBuffer();

		const std::vector<LightComponent*>& lights = world->getWorldComponent<GraphicsWorld>()->getLightManager()->getLights();
		std::vector<PointLightComponent*> pointLights;

		for (auto it : lights)
		{
			if (it->isA(PointLightComponent::getStaticTypeInfo()))
				pointLights.push_back(dynamicCast<PointLightComponent>(it));
		}

		if (pointLights.empty())
		{
			Core::msg(" GLRenderer::updatePointLightsConstants: Point lights vector is empty");
			g_renderDevice->setConstantBuffer(0);
			return;
		}

		if (pointLights.size() >= MAX_POINT_LIGHTS)
		{
			Core::msg(" GLRenderer::updatePointLightsConstants: pointLights.size() >= MAX_POINT_LIGHTS !!!");
			g_renderDevice->setConstantBuffer(0);
			return;
		}

		// get gpu data
		PointLightCB* pointLightData = (PointLightCB*)pointLightCB->map(BufferMapping::WriteOnly);

		int lightCounter = 0;
		for (auto it : pointLights)
		{
			if (lightCounter >= MAX_POINT_LIGHTS)
			{
				Core::msg(" GLRenderer::updatePointLightsConstants: lightCounter >= MAX_POINT_LIGHTS !!!");
				break;
			}

			pointLightData[lightCounter].color = glm::vec4(it->m_color, 1.0f);
			pointLightData[lightCounter].position = glm::vec4(it->m_node->getPosition(), 1.0f);
			pointLightData[lightCounter].specular = glm::vec4(it->m_specularColor, 1.0f);
			pointLightData[lightCounter].lightData.r = it->m_radius;

			lightCounter++;
		}

		// set buffer
		g_renderDevice->setConstantBufferIndex(ContantBuffersBinding::PointLights, pointLightCB);
#endif
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

		m_takeScreenshot = false;
	}

	void GLRenderer::clearScreen()
	{
	}

	void GLRenderer::clearRenderTarget(IRenderTarget* renderTarget)
	{
	}

	void GLRenderer::setSwapChainRenderTarget()
	{
	}

}
