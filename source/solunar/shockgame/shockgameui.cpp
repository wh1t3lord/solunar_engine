#include "shockgamepch.h"
#include "shockgame/shockgameui.h"

#include "core/file/contentmanager.h"

#include "engine/engine.h"
#include "engine/camera.h"

#include "graphics/texturemap.h"
#include "graphics/imguimanager.h"

#include "main/main.h"

namespace engine
{

static std::weak_ptr<TextureMap> g_mainMenuBackground;

ShockGameMainMenuComponent::ShockGameMainMenuComponent()
{
}

ShockGameMainMenuComponent::~ShockGameMainMenuComponent()
{
}

void ShockGameMainMenuComponent::onWorldSet(World* world)
{
	Component::onWorldSet(world);
	 
	g_mainMenuBackground = g_contentManager->loadObject<TextureMap>("textures/ui/ui_menu_background.png");
}

void ShockGameMainMenuComponent::update(float dt)
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

	CameraProxy* camera = CameraProxy::getInstance();

	ImGui::SetNextWindowPos(ImVec2(100, 550));
	ImGui::SetNextWindowSize(ImVec2(camera->getView()->m_width / 2, camera->getView()->m_height / 2));

	ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

	const ImVec2 kButtonSize = ImVec2(128.0f, 32.0f);

	if (ImGui::Button("New Game", kButtonSize))
	{
	//	loadLevel("env_test");
		EngineStateManager::getInstance()->loadWorld("worlds/env_test.xml");
	}

	if (ImGui::Button("Quit", kButtonSize))
	{
		//glfwSetWindowShouldClose(g_engineWindow, GLFW_TRUE);
		appFireExit();
	}

	ImGui::End();
}

}
