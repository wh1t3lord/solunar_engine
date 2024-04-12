#include "enginepch.h"
#include "core/file/contentmanager.h"
#include "engine/engine.h"
#include "engine/camera.h"
#include "engine/entity/entity.h"
#include "engine/entity/component.h"
#include "engine/entity/world.h"
#include "engine/entity/cameracomponent.h"
#include "engine/entity/logiccomponent.h"
#include "engine/physics/shapescomponent.h"
#include "engine/physics/rigidbodycomponent.h"
#include "engine/physics/trianglemeshshape.h"

#include "graphics/graphics.h"
#include "graphics/graphicsoptions.h"
#include "graphics/imguimanager.h"
#include "graphics/renderer.h"

namespace engine
{
	// #TODO: REMOVE
	ENGINE_API IGameInterface* g_gameInterface = nullptr;

	void registerEngineObjects()
	{
		// TODO: Rewrite to Class::registerObject()

		TypeManager::getInstance()->registerObject<Entity>();
		TypeManager::getInstance()->registerObject<Component>();
		TypeManager::getInstance()->registerObject<World>();

		TypeManager::getInstance()->registerObject<CameraComponent>();
		TypeManager::getInstance()->registerObject<CameraYawPitchRollComponent>();
		TypeManager::getInstance()->registerObject<CameraFirstPersonComponent>();

		TypeManager::getInstance()->registerObject<LogicComponent>();

		// Physics
		ShapeComponent::registerObject();
		BoxShapeComponent::registerObject();
		SphereShapeComponent::registerObject();
		CylinderShapeComponent::registerObject();
		CapsuleShapeComponent::registerObject();
		TriangleMeshShapeComponent::registerObject();
		RigidBodyComponent::registerObject();
		RigidBodyProxyComponent::registerObject();

		/*Node::registerObject();
		CameraNode::registerObject();

		Component::registerObject();
		TransformComponent::registerObject();
		LogicComponent::registerObject();


		WorldComponent::registerObject();
		WorldEnvironmentComponent::registerObject();

		World::registerObject();
		LoadingRoomManager::registerObject();*/
	}

	bool g_harakiriLogicThread = false;
	bool g_worldIsReady = false;

	class LogicThread : public Thread
	{
	public:
		LogicThread();
		~LogicThread();

		void execute() override;
	};

	LogicThread::LogicThread()
	{
	}

	LogicThread::~LogicThread()
	{
		g_harakiriLogicThread = true;
	}

	void LogicThread::execute()
	{
		setThreadName("Logic Thread");

		while (!g_harakiriLogicThread)
		{
			if (g_worldIsReady)
			{
				Assert2(Engine::ms_world, "Trying to update nullptr world. Check g_worldIsReady!");

				Engine::ms_world->updateLogicWorld();
			}

			Sleep(10);
		}

		Core::msg("LogicThread: exiting ...");
	}

	LogicThread g_logicThread;

	World* Engine::ms_world = nullptr;

	void Engine::init()
	{
		Core::msg("Initializing engine");

		registerEngineObjects();

		g_cameraProxy = mem_new<CameraProxy>();
		
		// Start logic thread
//		g_logicThread.start();

		//ScriptManager::getInstance()->init();

		// initalize console
		//g_console->init();

		//WorldManager::init();

		//g_harakiriLogicThread = true;
		//g_logicThread.stop();
	}

	void Engine::shutdown()
	{
		// harakiri logic thread
	//	g_harakiriLogicThread = true;
	//	g_logicThread.stop();

		if (ms_world)
		{
			Core::msg("Engine: world is present on engine shutdown, deleting ...");

			mem_delete(ms_world);
			ms_world = nullptr;
		}

		// shutdown console
	//	g_console->shutdown();

		//ScriptManager::getInstance()->shutdown();

		mem_delete(g_cameraProxy);
		g_cameraProxy = nullptr;
	}

	void Engine::loadWorld(const std::string& filename)
	{
		if (ms_world)
		{
			mem_delete(ms_world);
			ms_world = nullptr;
		}

		Core::msg("Engine::loadWorld() Loading %s", filename.c_str());

		std::shared_ptr<DataStream> stream = g_contentManager->openStream(filename);

		stream->seek(Seek_End, 0);
		size_t length = stream->tell();
		stream->seek(Seek_Begin, 0);

		char* data = new char[length + 1];
		stream->read((void*)data, length);
		data[length] = '\0';

		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError error = doc.Parse(data, length);
		
		if (error != tinyxml2::XML_SUCCESS)
		{
			Core::error("Engine::loadWorld() Failed to parse world %s.", filename.c_str());
		}
		
		tinyxml2::XMLElement* worldElement = doc.FirstChildElement("World");;

		World* world = g_typeManager->createObject<World>();
		world->loadXML(*worldElement);
		
		ms_world = world;
		
		delete[] data;
	}

	void Engine::loadEmptyWorld()
	{
		if (ms_world)
		{
			mem_delete(ms_world);
			ms_world = nullptr;
		}

		Core::msg("Engine::loadEmptyWorld() Creating ...");

		World* world = g_typeManager->createObject<World>();
		ms_world = world;
	}

	void Engine::update()
	{
		//OPTICK_EVENT("Engine::update");

		EngineStateManager::getInstance()->update();

		//g_eventManager.update();

		World* world = Engine::ms_world;
		if (world)
		{
			world->update_PreEntityUpdate();
			world->updatePhysicsWorld();
			world->updateLogicWorld();
		}
	}

	void Engine::setGameInterface(IGameInterface* gameInterface)
	{
	}

	IGameInterface* Engine::getGameInterface()
	{
		return nullptr;
	}

	static EngineStateManager s_engineStateManager;

	EngineStateManager::EngineStateManager()
	{
		m_currentState = EngineState::None;
		m_nextState = EngineState::None;
	}

	EngineStateManager::~EngineStateManager()
	{
	}

	void EngineStateManager::loadWorld(const std::string& filename)
	{
		m_nextState = EngineState::LoadWorld;
		m_worldName = filename;
	}

	void EngineStateManager::loadEmptyWorld()
	{
		m_nextState = EngineState::LoadWorld;
		m_worldName.clear();
	}

	void EngineStateManager::update()
	{
		m_currentState = m_nextState;
		switch (m_currentState)
		{
		case EngineState::Running:
			break;
		case EngineState::LoadWorld:
			if (m_worldName.empty())
				Engine::loadEmptyWorld();
			else
				Engine::loadWorld(m_worldName);

			m_nextState = EngineState::Running;

			break;
		
		default:
			break;
		}
	}

	void GraphicsFacade::init(void* window)
	{
		graphicsInit(window);
	}

	void GraphicsFacade::shutdown()
	{
		graphicsShutdown();
	}

	bool GraphicsFacade::loadSettings(const std::string& filename)
	{
		return g_graphicsOptions.loadSettings(filename);
	}

	void GraphicsFacade::saveSettings(const std::string& filename)
	{
		g_graphicsOptions.saveSettings(filename);
	}

	void GraphicsFacade::applyDefaultOptions()
	{
		g_graphicsOptions.applyDefaultOptions();
	}

	int GraphicsFacade::getWidth()
	{
		return g_graphicsOptions.m_width;
	}

	int GraphicsFacade::getHeight()
	{
		return g_graphicsOptions.m_height;
	}

	bool GraphicsFacade::getFullscreen()
	{
		return g_graphicsOptions.m_fullscreen;
	}

	IImGuiManager* GraphicsFacade::getImGuiManager()
	{
		return (IImGuiManager*)ImGuiManager::getInstance();
	}

	IRenderer* GraphicsFacade::getRenderer()
	{
		return g_renderer;
	}

}