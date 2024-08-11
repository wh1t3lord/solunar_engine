#include "shockgamepch.h"
#include "shockgame/shockgameui.h"

#include "core/file/contentmanager.h"

#include "engine/engine.h"
#include "engine/camera.h"

#include "engine/audio/musicmanager.h"

#include "graphics/texturemap.h"
#include "graphics/imguimanager.h"
#include "graphics/graphicsoptions.h"

#include "main/main.h"

namespace solunar
{

static std::weak_ptr<TextureMap> g_mainMenuBackground;

ShockGameMainMenuComponent::ShockGameMainMenuComponent()
{
}

ShockGameMainMenuComponent::~ShockGameMainMenuComponent()
{
}

void ShockGameMainMenuComponent::OnWorldSet(World* world)
{
	Component::OnWorldSet(world);
	 
	g_mainMenuBackground = g_contentManager->LoadObject<TextureMap>("textures/ui/ui_menu_background.png");

	MusicManager::GetInstance()->Play("sounds/music/temp_mainmenu.mp3", true);
}

void ShockGameMainMenuComponent::Update(float dt)
{
	// begin menu window

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(io.DisplaySize);
	if (ImGui::Begin("Main Menu Background", (bool*)true, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs))
	{
		ImGui::Image(g_mainMenuBackground.lock()->getHWTexture(), io.DisplaySize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::End();
	}

	CameraProxy* camera = CameraProxy::GetInstance();

	ImGui::SetNextWindowPos(ImVec2(100, 550));
	ImGui::SetNextWindowSize(ImVec2(camera->GetView()->m_width / 2, camera->GetView()->m_height / 2));

	ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

	const ImVec2 kButtonSize = ImVec2(128.0f, 32.0f);

	if (ImGui::Button("New Game", kButtonSize))
	{
	//	loadLevel("env_test");

		MusicManager::GetInstance()->Stop();
		EngineStateManager::GetInstance()->LoadWorld("worlds/env_test.xml");
	}

	if (ImGui::Button("Quit", kButtonSize))
	{
		//glfwSetWindowShouldClose(g_engineWindow, GLFW_TRUE);
		appFireExit();
	}

	ImGui::End();
}

/////////////////////////////////////////////////////////////////////
// DEMO GAME

class VideoModeManager
{
public:
	struct VideoMode
	{
		std::string name;
		int width, height, refreshRate;
	};
public:
	static bool ms_inited;
	static int ms_currentVidMode;
	static std::vector<VideoMode> ms_vidModes;
public:
	static void Init();
};

bool                                     VideoModeManager::ms_inited = false;
int                                      VideoModeManager::ms_currentVidMode = 0;
std::vector<VideoModeManager::VideoMode>          VideoModeManager::ms_vidModes;

void VideoModeManager::Init()
{
	VideoMode videoMode;
	videoMode.width = g_graphicsOptions.m_width;
	videoMode.height = g_graphicsOptions.m_height;
	videoMode.refreshRate = g_graphicsOptions.m_refreshRate;
	
	char buf[64];
	snprintf(buf, sizeof(buf), "%ix%i (%i)", videoMode.width, videoMode.height, videoMode.refreshRate);
	videoMode.name = buf;

	ms_vidModes.push_back(videoMode);

	//GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	//const GLFWvidmode* videoModes = glfwGetVideoModes(primaryMonitor, &ms_vidModeCount);

	//for (int i = 0; i < ms_vidModeCount; i++)
	//{
	//	spdlog::info("{} {} {}", videoModes[i].width, videoModes[i].height, videoModes[i].refreshRate);

	//	int refresh = videoModes[i].refreshRate;
	//	if (refresh != 60 && refresh != 70 && refresh != 75)
	//		continue;

	//	const GLFWvidmode& vidMode = videoModes[i];

	//	VideoMode mode;
	//	mode.width = vidMode.width;
	//	mode.height = vidMode.height;
	//	mode.refreshRate = vidMode.refreshRate;

	//	char buffer[256];
	//	sprintf(buffer, "%ix%i %i", mode.width, mode.height, mode.refreshRate);
	//	mode.name = buffer;

	//	ms_vidModes.push_back(mode);
	//}


	for (int i = 0; i < ms_vidModes.size(); i++)
	{
		GraphicsOptions* opts = &g_graphicsOptions;

		if (opts->m_width == ms_vidModes[i].width &&
			opts->m_height == ms_vidModes[i].height &&
			opts->m_refreshRate == ms_vidModes[i].refreshRate)
		{
			ms_currentVidMode = i;
			break;
		}
	}

	ms_inited = true;
}

static bool s_showSettingsMenu = false;

void showSettingsMenu()
{
	ImGui::Begin("Settings", &s_showSettingsMenu);

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Video"))
		{
			if (!VideoModeManager::ms_inited)
				VideoModeManager::Init();

			static int item_current_idx = VideoModeManager::ms_currentVidMode; // Here we store our selection data as an index.
			const char* combo_preview_value = VideoModeManager::ms_vidModes[item_current_idx].name.c_str();
			if (ImGui::BeginCombo("Video Mode", combo_preview_value))
			{
				for (int i = 0; i < VideoModeManager::ms_vidModes.size(); i++)
				{

					const bool is_selected = (item_current_idx == i);
					if (ImGui::Selectable(VideoModeManager::ms_vidModes[i].name.c_str(), is_selected))
						item_current_idx = i;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Graphics"))
		{
			ImGui::SliderInt("Texture Quality", &g_graphicsOptions.m_texturesMipMapLevel, 0, 4);
			ImGui::SliderInt("Anisotropic Quality", &g_graphicsOptions.m_anisotropicQuality, 0, 16);
			ImGui::SliderInt("Shadows Quality", &g_graphicsOptions.m_shadowsQuality, 0, 4);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Input"))
		{
			static float s_mouseSensitivy = 1.0f;
			ImGui::SliderFloat("Mouse Sensitivity", &s_mouseSensitivy, 0.1f, 10.0f);

			static bool s_mouseInvert = false;
			ImGui::Checkbox("Mouse Invert", &s_mouseInvert);

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
}

DemoGameMainMenuComponent::DemoGameMainMenuComponent()
{
}

DemoGameMainMenuComponent::~DemoGameMainMenuComponent()
{
}

void DemoGameMainMenuComponent::OnWorldSet(World* world)
{
	Component::OnWorldSet(world);

	g_mainMenuBackground = g_contentManager->LoadObject<TextureMap>("textures/ui/ui_menu_background.png");

	//MusicManager::GetInstance()->play("sounds/music/temp_mainmenu.mp3", true);
}

void DemoGameMainMenuComponent::Update(float dt)
{
	// begin menu window

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(io.DisplaySize);
	if (ImGui::Begin("Main Menu Background", (bool*)true, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs))
	{
		ImGui::Image(g_mainMenuBackground.lock()->getHWTexture(), io.DisplaySize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::End();
	}

	CameraProxy* camera = CameraProxy::GetInstance();

	ImGui::SetNextWindowPos(ImVec2(100, 550));
	ImGui::SetNextWindowSize(ImVec2(camera->GetView()->m_width / 2, camera->GetView()->m_height / 2));

	ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

	const ImVec2 kButtonSize = ImVec2(128.0f, 32.0f);

	if (ImGui::Button("New Game", kButtonSize))
	{
		MusicManager::GetInstance()->Stop();
		EngineStateManager::GetInstance()->LoadWorld("worlds/env_test.xml");
	}

	if (ImGui::Button("Settings", kButtonSize))
	{
		s_showSettingsMenu = true;
	}

	if (ImGui::Button("Quit", kButtonSize))
	{
		appFireExit();
	}

	ImGui::End();

	if (s_showSettingsMenu)
		showSettingsMenu();
}


}
