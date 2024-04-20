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
#include "graphics/viewsurface.h"

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
	static bool g_showLightEditor = true;
	static bool g_forceQuit = false;
	static bool g_showShockPlayerDebug = false;

	void EngineLoop::initialize()
	{
		initEngineCommandLine();

		// initialize engine
		Engine::init();
		
		// Initialize graphics
		graphicsInit();

		// create engine view
		createEngineView();
		
		// initialize audio manager
		//AudioManager* pAudioManager = AudioManager::createInstance();
		//pAudioManager->init();

		// Game interface init
		g_gameInterface->initialize();

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
	}

	void EngineLoop::shutdown()
	{
		g_gameInterface->shutdown();

		//AudioManager::getInstance()->shutdown();
		//AudioManager::destroyInstance();

		// release content manager (because some objects allocated by renderer, and after
		//							renderer destroying, render device is unavaliable)
		g_contentManager->shutdown();

		graphicsShutdown();

		g_graphicsOptions.saveSettings("engine.ini");

		Engine::shutdown();
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
		
		// update camera
		CameraProxy::getInstance()->updateProxy();

		// run engine frame
		Engine::update();

		// sound
		//AudioManager::getInstance()->update();

		// begin render frame
		g_renderer->beginFrame();
		
		// get vsync value
		int frameSyncNum = g_graphicsOptions.m_vsync;

		// show display
		g_renderer->renderView(appGetViewSurface(), frameSyncNum);

		// end render frame
		g_renderer->endFrame();

		return true;
	}

}
