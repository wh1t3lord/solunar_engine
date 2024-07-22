#include "pch.h"

#include "main/engineloop.h"
#include "main/main.h"

#include "core/timer.h"
#include "core/utils/iniFile.h"
#include "core/file/contentmanager.h"

#include "engine/engine.h"
#include "engine/inputmanager.h"
#include "engine/gameinterface.h"
#include "engine/camera.h"
#include "engine/entity/world.h"
#include "engine/audio/audiomanager.h"

#include "graphics/graphics.h"
#include "graphics/graphicsoptions.h"
#include "graphics/renderer.h"
#include "graphics/imguimanager.h"
#include "graphics/lightmanager.h"

#include "shockgame/shockgame.h"

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
	static bool g_showLightEditor = false;
	static bool g_forceQuit = false;
	static bool g_showShockPlayerDebug = false;

	// #TODO: SO WRONG
	void appFireExit()
	{
		g_forceQuit = true;
	}

	void engineDebugOverlay()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Engine"))
			{
				if (ImGui::MenuItem("Toggle Physics Debug Draw")) { if (Engine::ms_world) Engine::ms_world->togglePhysicsDebugDraw(); }
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

		if (g_showShockPlayerDebug)
			shockGamePlayerDebug(&g_showShockPlayerDebug);
	}

	static std::string g_startWorldFilename;

	void loadEnvironmentConfig()
	{
		CIniFile environment("environment.ini");
		if (!environment.ReadFile())
			return; // defaults

		g_startWorldFilename = environment.GetValue("Global_PC", "StartWorld");
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
		AudioManager* audioManager = AudioManager::createInstance();
		audioManager->init();

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

		loadEnvironmentConfig();

		if (!g_startWorldFilename.empty() && !g_commandLine.hasOption("-world"))
		{
			char stringBuffer[256];
			snprintf(stringBuffer, sizeof(stringBuffer), "worlds/%s.xml", g_startWorldFilename.c_str());
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

		AudioManager::getInstance()->shutdown();
		AudioManager::destroyInstance();

		//ImguiManager::getInstance()->shutdown();

		// release content manager (because some objects allocated by renderer, and after
		//							renderer destroying, render device is unavaliable)
		g_contentManager->shutdown();

		graphicsShutdown();

		g_graphicsOptions.saveSettings("GameSettings.ini");

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

		// update timer
		Timer::getInstance()->update();

		// Begin ImGui frame
		ImGuiManager::getInstance()->beginFrame();

		// update camera
		CameraProxy::getInstance()->updateProxy();

		// run engine frame
		Engine::update();

		// sound
		AudioManager::getInstance()->update();

		// install current view
		g_renderer->setView(CameraProxy::getInstance()->getView());

		// Begin renderer frame
		g_renderer->beginFrame();
		
		// Render view
		g_renderer->renderView(appGetView());

#ifndef FINAL_BUILD
		// Draw the engine debug overlay
		engineDebugOverlay();
#endif // !FINAL_BUILD
		
		// End and render ImGui
		ImGuiManager::getInstance()->endFrame();

		// End renderer frame
		g_renderer->endFrame();

		// Take screenshot
		if (input->getKeyWithReset(KeyboardKeys::KEY_F12))
			g_renderer->takeScreenshot();

		return true;
	}

}
