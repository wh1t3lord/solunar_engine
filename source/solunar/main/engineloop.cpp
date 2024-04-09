#include "pch.h"

#include "main/engineloop.h"
#include "main/main.h"

#include "core/timer.h"
#include "core/file/contentmanager.h"

#include "engine/engine.h"
#include "engine/inputmanager.h"
#include "engine/gameinterface.h"
#include "engine/camera.h"

#include "graphics/graphics.h"
#include "graphics/graphicsoptions.h"
#include "graphics/renderer.h"
#include "graphics/imguimanager.h"
#include "graphics/lightmanager.h"

//#include "shockgame/shockgame.h"

namespace engine {

	extern void graphicsShowConstantBuffers(bool* open);

	// extern View* g_engineView;
	
	// Debug purposes global variables
	bool g_quitAtStart = false;
	bool g_slowdown = false;
	bool g_slowdown2X = false;

	// #TODO: REMOVE
	bool g_modelConvert = false;
	std::string g_modelConvertName;

	void initEngineCommandLine()
	{
		if (g_commandLine.hasOption("-quit"))
			g_quitAtStart = true;

		if (g_commandLine.hasOption("-slowdown"))
			g_slowdown = true;

		if (g_commandLine.hasOption("-slowdown2x"))
			g_slowdown2X = true;

		if (g_commandLine.hasOption("-modelConvert"))
		{
			g_modelConvert = true;
			g_modelConvertName = g_commandLine.getOptionParameter("-modelConvert");
		}
	}

	static bool g_showEntityList = false;
	static bool g_showCBManager = false;
	static bool g_showLightEditor = true;
	static bool g_forceQuit = false;
	static bool g_showShockPlayerDebug = false;

	void engineDebugOverlay()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Engine"))
			{
				if (ImGui::MenuItem("Entity list")) { g_showEntityList = !g_showEntityList; }
				if (ImGui::MenuItem("Quit")) { g_forceQuit = true; }

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Graphics"))
			{
				if (ImGui::MenuItem("Constant Buffer Tracker")) { g_showCBManager = !g_showCBManager; }
				if (ImGui::MenuItem("Light Editor")) { g_showLightEditor = !g_showLightEditor; }

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Game"))
			{
				if (ImGui::MenuItem("Shock Player Debug")) { g_showShockPlayerDebug = !g_showShockPlayerDebug; }

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Debug"))
			{
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (g_showCBManager)
			graphicsShowConstantBuffers(&g_showCBManager);
	
		if (g_showLightEditor)
			graphicsLightEditor(&g_showLightEditor);

		//if (g_showShockPlayerDebug)
			//sshockGamePlayerDebug(&g_showShockPlayerDebug);
	}

	void EngineLoop::initialize()
	{
		initEngineCommandLine();

		//appInit2();

		// create engine view
		createEngineView();
		//appInitInput();

		// initialize engine
		Engine::init();
		
		graphicsInit();
		
		// initialize audio manager
		//AudioManager* pAudioManager = AudioManager::createInstance();
		//pAudioManager->init();

		// game init
		//Game::init();

		// Game interface init
		g_gameInterface->initialize();

		//if (g_commandLine.hasOption("-saveClassIds"))
		//	saveClassIds();

		//if (g_modelConvert)
		//{
		//	std::shared_ptr<ModelBase> model = g_contentManager->loadModelOld(g_modelConvertName);
		//	model->saveBinary(g_modelConvertName);
		//}

		if (g_commandLine.hasOption("-world"))
		{
			const char* worldFileName = g_commandLine.getOptionParameter("-world");
			char stringBuffer[256];
			snprintf(stringBuffer, sizeof(stringBuffer), "worlds/%s.xml", worldFileName);
			EngineStateManager::getInstance()->loadWorld(stringBuffer);
		}

		//if (g_commandLine.hasOption("-world"))
		//{
		//	// #TODO: Refactor this
		//	GameState* gameState = GameState::getInstance();
		//	gameState->setGameState(GameState::GAME_STATE_RUNNING);
		//	loadLevel();
		//}
		//else
		//{
		//	// #TODO: Refactor this
		//	GameState* gameState = GameState::getInstance();
		//	gameState->setGameState(GameState::GAME_STATE_RUNNING);
		//	loadLevel("entry");
		//}
	}

	void EngineLoop::shutdown()
	{
		g_gameInterface->shutdown();

		//Game::shutdown();

		//AudioManager::getInstance()->shutdown();
		//AudioManager::destroyInstance();

		//ImguiManager::getInstance()->shutdown();

		// release content manager (because some objects allocated by renderer, and after
		//							renderer destroying, render device is unavaliable)
		g_contentManager->shutdown();

		graphicsShutdown();

		g_graphicsOptions.saveSettings("engine.ini");

		Engine::shutdown();

		//appShutdown2();
	}

	bool EngineLoop::update()
	{
//		OPTICK_EVENT("EngineLoop::update");

		InputManager* input = InputManager::getInstance();

		if (g_quitAtStart)
			return true;

		if (g_forceQuit)
			return false;

		if (g_slowdown)
			Sleep(100);

		if (g_slowdown2X)
			Sleep(200);
			
		// update delta cursor pos and others input stuff
		input->update();

//		glfwPollEvents();
//
//		if (gameState->getGameState() == GameState::GAME_STATE_RUNNING &&
//			!input->getKey(KeyboardKeys::KEY_LEFT_ALT))
//		{
//			appToggleShowMousePointer(false);
//			//appSetCursorPos(0, 0);
//		}
//		else
//		{
//			appToggleShowMousePointer(true);
//		}

		// update timer
		Timer::getInstance()->update();

		ImGuiManager::getInstance()->beginFrame();

		// update game specific state
		//gameState->update();
		
		// update camera
		CameraProxy::getInstance()->updateProxy();

		// run engine frame
		Engine::update();

		// sound
		//AudioManager::getInstance()->update();

		g_renderer->beginFrame();
		
		g_renderer->renderView(appGetView());

//		if (g_console->isToggled())
//			g_console->onRender();

//#if 1
//		DebugOverlay::render();
//#endif // !MASTER_GOLD_BUILD

//		RmlSystem::getInstance()->render();

		engineDebugOverlay();

		ImGui::ShowDemoWindow();

		ImGuiManager::getInstance()->endFrame();

		g_renderer->endFrame();
//
//		//appPresent();
//
//		Sleep(1);
		return true;
	}

}
