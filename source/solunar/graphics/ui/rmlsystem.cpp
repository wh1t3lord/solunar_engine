#include "graphicspch.h"

#if 0
#include "core/timer.h"

#include "graphics/ui/rmlsystem.h"
#include "graphics/core/device.h"

#define RMLUI_GL3_CUSTOM_LOADER <glad/glad.h>
#include <../../../externals/RmlUi-5.1/Backends/RmlUi_Renderer_GL3.h>
#include <../../../externals/RmlUi-5.1/Backends/RmlUi_Renderer_GL3.cpp>

#include <../../../externals/RmlUi-5.1/Backends/RmlUi_Platform_GLFW.h>
#include <../../../externals/RmlUi-5.1/Backends/RmlUi_Platform_GLFW.cpp>

#include <GLFW/glfw3.h>

namespace solunar
{

	// #TODO: !!! REMOVE !!!
	// #TODO: !!! UGLY HACK !!!
	extern GLFWwindow* g_engineWindow;

class RmlSystemInteface : public SystemInterface_GLFW
{
public:
	static RmlSystemInteface* getInstance()
	{
		static RmlSystemInteface instance;
		return &instance;
	}

public:
	double GetElapsedTime() override
	{
		return (double)Timer::getInstance()->getTime();
	}

	bool LogMessage(Rml::Log::Type type, const Rml::String& message) override
	{
		std::string logType = "Unknown";

		switch (type)
		{
		case Rml::Log::LT_ALWAYS: logType = "[always]"; break;
		case Rml::Log::LT_ERROR: logType = "[error]"; break;
		case Rml::Log::LT_ASSERT: logType = "[assert]"; break;
		case Rml::Log::LT_WARNING: logType = "[warning]"; break;
		case Rml::Log::LT_INFO: logType = "[info]"; break;
		case Rml::Log::LT_DEBUG: logType = "[debug]"; break;
		default:
			break;
		}

		Core::msg("[RmlUI]: %s: %s", logType.c_str(), message.c_str());

		return true;
	}
};

class RmlFontEngineInterface : public Rml::FontEngineInterface
{

};

// global instance
RmlSystem RmlSystem::ms_instance;

static RenderInterface_GL3* g_renderInterface_GL3 = nullptr;

RmlSystem::RmlSystem() :
	m_filesystem(nullptr),
	m_renderer(nullptr),
	m_context(nullptr)
{
}

RmlSystem::~RmlSystem()
{
}

void RmlSystem::init()
{
	// create file system and renderer
	m_filesystem = mem_new<RmlFileSystem>();

	m_renderer = mem_new<RmlRenderer>();
	m_renderer->init();

	// assing
	Rml::SetSystemInterface(RmlSystemInteface::getInstance());
	Rml::SetFileInterface(m_filesystem);
	Rml::SetRenderInterface(m_renderer);

	//RmlGL3::Initialize();
	//
	//g_renderInterface_GL3 = mem_new<RenderInterface_GL3>();
	//Rml::SetRenderInterface(g_renderInterface_GL3);
	//
	//int width= 0, height = 0;
	//glfwGetFramebufferSize(g_engineWindow, &width, &height);
	//g_renderInterface_GL3->SetViewport(width, height);

	// Initialize Rml
	Rml::Initialise();
}

void RmlSystem::shutdown()
{
	// Shutdown rml
	Rml::Shutdown();

	// destroy everything
	m_renderer->shutdown();
	mem_delete(m_renderer); m_renderer = nullptr;
	mem_delete(m_filesystem); m_filesystem = nullptr;
}

void RmlSystem::createContext(int width, int height)
{
	ASSERT2(!m_context, "Context re-creation doesn't implemented yet!");
	m_context = Rml::CreateContext("main", Rml::Vector2i(width, height));

	Rml::Vector2i window_size;
	float dp_ratio = 1.f;
	glfwGetFramebufferSize(g_engineWindow, &window_size.x, &window_size.y);
	glfwGetWindowContentScale(g_engineWindow, &dp_ratio, nullptr);

	m_context->SetDimensions(window_size);
	m_context->SetDensityIndependentPixelRatio(dp_ratio);

	// Load fonts

	const Rml::String directory = "ui/fonts/Robot_Mono/";

	struct FontFace {
		const char* filename;
		bool fallback_face;
	};
	FontFace font_faces[] = {
		{"RobotoMono-Regular.ttf", false},
		{"RobotoMono-Italic.ttf", false},
		{"RobotoMono-Bold.ttf", false},
		{"RobotoMono-BoldItalic.ttf", false}
	};

	for (const FontFace& face : font_faces)
		Rml::LoadFontFace(directory + face.filename, face.fallback_face);
}

void RmlSystem::render()
{
	OPTICK_EVENT("RmlSystem::render");

	m_context->Update();

	m_renderer->beginFrame();
	m_context->Render();
	m_renderer->endFrame();

	//m_context->Update();

	//g_renderDevice->setTexture2D(0, nullptr);
	//g_renderDevice->setTexture2D(1, nullptr);
	//g_renderDevice->setTexture2D(2, nullptr);
	//g_renderDevice->setTexture2D(3, nullptr);

	//g_renderInterface_GL3->BeginFrame();
	//g_renderInterface_GL3->Clear();

	//m_context->Render();

	//g_renderInterface_GL3->EndFrame();
}

Rml::Context* RmlSystem::getContext()
{
	ASSERT(m_context);
	return m_context;
}

}
#endif
