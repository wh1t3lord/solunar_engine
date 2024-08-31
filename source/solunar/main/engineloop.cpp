#include "pch.h"

#include "main/engineloop.h"
#include "main/main.h"

#include "core/timer.h"
#include "core/utils/iniFile.h"
#include "core/file/contentmanager.h"
#include "core/object/typemanager.h"
#include "core/object/propertymanager.h"

#include "engine/engine.h"
#include "engine/inputmanager.h"
#include "engine/gameinterface.h"
#include "engine/camera.h"
#include "engine/entity/component.h"
#include "engine/entity/world.h"
#include "engine/audio/audiomanager.h"

#include "graphics/graphics.h"
#include "graphics/graphicsoptions.h"
#include "graphics/renderer.h"
#include "graphics/imguimanager.h"
#include "graphics/lightmanager.h"

#include "shockgame/shockgame.h"
#include "shockgame/shockplayercontroller.h"

namespace solunar {

	extern void graphicsShowConstantBuffers(bool* open);

	// extern View* g_engineView;
	
	// Debug purposes global variables
	bool g_quitAtStart = false;
	bool g_slowdown = false;
	bool g_slowdown2X = false;

	// #TODO: REMOVE
	bool g_modelConvert = false;
	std::string g_modelConvertName;

	void InitEngineCommandLine()
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

	void ExportClassesForEditor()
	{
		tinyxml2::XMLDocument doc;

		tinyxml2::XMLNode* pRoot = doc.NewElement("Classes");
		doc.InsertFirstChild(pRoot);

		std::vector<const TypeInfo*> registeredTypes;
		g_typeManager->GetRegisteredTypes(registeredTypes);

		for (auto it : registeredTypes)
		{
			if (!it->IsA<Component>())
				continue;

			tinyxml2::XMLElement* component = doc.NewElement("Class");
			component->SetAttribute("classname", it->GetClassName());
			
			if (it->GetBaseInfo())
				component->SetAttribute("baseClassname", it->GetBaseInfo()->GetClassName());
			
			pRoot->InsertFirstChild(component);

			std::vector<IProperty*> properties;
			PropertyManager::GetInstance()->GetProperties(it, properties);
			if (!properties.empty())
			{
				tinyxml2::XMLElement* propertiesElement = doc.NewElement("Properties");
				component->InsertFirstChild(propertiesElement);

				for (auto prop : properties)
				{
					tinyxml2::XMLElement* propertyElement = doc.NewElement("Property");
					propertiesElement->InsertFirstChild(propertyElement);

					propertyElement->SetAttribute("name", prop->GetName());

					PropertyType propertyType = prop->GetType();
					switch (propertyType)
					{
					case PropertyType_Bool:
						propertyElement->SetAttribute("type", "Bool");
						break;
					case PropertyType_Integer:
						propertyElement->SetAttribute("type", "Integer");
						break;
					case PropertyType_Float:
						propertyElement->SetAttribute("type", "Float");
						break;
					case PropertyType_Vector2:
						propertyElement->SetAttribute("type", "Vector2");
						break;
					case PropertyType_Vector3:
						propertyElement->SetAttribute("type", "Vector3");
						break;
					case PropertyType_Vector4:
						propertyElement->SetAttribute("type", "Vector4");
						break;
					case PropertyType_Quaternion:
						propertyElement->SetAttribute("type", "Quaternion");
						break;
					case PropertyType_Matrix4x4:
						propertyElement->SetAttribute("type", "Matrix4x4");
						break;
					case PropertyType_BoundingBox:
						propertyElement->SetAttribute("type", "BoundingBox");
						break;
					case PropertyType_String:
						propertyElement->SetAttribute("type", "String");
						break;
					default:
						Assert2(0, "!!!");
						break;
					}
				}
			}
		}

		doc.SaveFile("editor_classes.xml");
	}

	void ShowEngineDebugOverlay()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Engine"))
			{
				if (ImGui::MenuItem("Toggle Physics Debug Draw")) { if (Engine::ms_world) Engine::ms_world->TogglePhysicsDebugDraw(); }
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

	void LoadEnvironmentConfig()
	{
		CIniFile environment("environment.ini");
		if (!environment.ReadFile())
			return; // defaults

		g_startWorldFilename = environment.GetValue("Global_PC", "StartWorld");
	}

	void EngineLoop::Initialize()
	{
		InitEngineCommandLine();

		//appInit2();

		// create engine view
		CreateEngineView();
		//appInitInput();

		// Initialize engine
		Engine::Init();
		
		graphicsInit();
		
		// Initialize audio manager
		AudioManager* audioManager = AudioManager::CreateInstance();
		audioManager->Init();

		// game init
		//Game::Init();

		// Game interface init
		g_gameInterface->Initialize();

		//if (g_commandLine.hasOption("-saveClassIds"))
		//	saveClassIds();

		//if (g_modelConvert)
		//{
		//	std::shared_ptr<ModelBase> model = g_contentManager->LoadModelOld(g_modelConvertName);
		//	model->saveBinary(g_modelConvertName);
		//}

		// Register properties #TODO: PLEASE MAKE IT MORE NORMAL
		PropertyRegistrator::GetInstance()->RegisterClasses();

		//ExportClassesForEditor();

		if (g_commandLine.hasOption("-world"))
		{
			const char* worldFileName = g_commandLine.getOptionParameter("-world");
			char stringBuffer[256];
			snprintf(stringBuffer, sizeof(stringBuffer), "worlds/%s.xml", worldFileName);
			EngineStateManager::GetInstance()->LoadWorld(stringBuffer);
		}

		LoadEnvironmentConfig();

		if (!g_startWorldFilename.empty() && !g_commandLine.hasOption("-world"))
		{
			char stringBuffer[256];
			snprintf(stringBuffer, sizeof(stringBuffer), "worlds/%s.xml", g_startWorldFilename.c_str());
			EngineStateManager::GetInstance()->LoadWorld(stringBuffer);
		}

		//if (g_commandLine.hasOption("-world"))
		//{
		//	// #TODO: Refactor this
		//	GameState* gameState = GameState::GetInstance();
		//	gameState->setGameState(GameState::GAME_STATE_RUNNING);
		//	loadLevel();
		//}
		//else
		//{
		//	// #TODO: Refactor this
		//	GameState* gameState = GameState::GetInstance();
		//	gameState->setGameState(GameState::GAME_STATE_RUNNING);
		//	loadLevel("entry");
		//}
	}

	void EngineLoop::Shutdown()
	{
		g_gameInterface->Shutdown();

		//Game::Shutdown();

		AudioManager::GetInstance()->Shutdown();
		AudioManager::DestroyInstance();

		//ImguiManager::GetInstance()->Shutdown();

		// Release content manager (because some objects allocated by renderer, and after
		//							renderer destroying, render device is unavaliable)
		g_contentManager->Shutdown();

		graphicsShutdown();

		g_graphicsOptions.SaveSettings("GameSettings.ini");

		Engine::Shutdown();

		//appShutdown2();
	}

	bool EngineLoop::Update()
	{
//		OPTICK_EVENT("EngineLoop::update");

		InputManager* input = InputManager::GetInstance();

		if (g_quitAtStart)
			return true;

		if (g_forceQuit)
			return false;

		if (g_slowdown)
			Sleep(100);

		if (g_slowdown2X)
			Sleep(200);
			
		// update delta cursor pos and others input stuff
		input->Update();

		// update timer
		Timer::GetInstance()->Update();

		// Begin ImGui frame
		ImGuiManager::GetInstance()->BeginFrame();

		// update camera
		CameraProxy::GetInstance()->UpdateProxy();

		// run engine frame
		Engine::Update();

		// sound
		AudioManager::GetInstance()->Update();

		// install current view
		g_renderer->SetView(CameraProxy::GetInstance()->GetView());

		// Begin renderer frame
		g_renderer->BeginFrame();
		
		// Render view
		g_renderer->RenderView(appGetView());

#ifndef FINAL_BUILD
		// Draw the engine debug overlay
		ShowEngineDebugOverlay();
#endif // !FINAL_BUILD
		
		// End and render ImGui
		ImGuiManager::GetInstance()->EndFrame();

		// End renderer frame
		g_renderer->EndFrame();

		// Take screenshot
		if (input->IsPressedWithReset(KeyboardKeys::KEY_F12))
			g_renderer->TakeScreenshot();

		return true;
	}

}
