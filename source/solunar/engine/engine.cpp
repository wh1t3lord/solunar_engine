#include "enginepch.h"
#include "core/file/contentmanager.h"
#include "engine/engine.h"
#include "engine/entity/entity.h"
#include "engine/entity/component.h"
#include "engine/entity/world.h"
#include "engine/entity/cameracomponent.h"
#include "engine/entity/logiccomponent.h"

#include "engine/physics/shapescomponent.h"
#include "engine/physics/rigidbodycomponent.h"
#include "engine/physics/trianglemeshshape.h"

namespace engine
{

	//void registerEngineObjects()
	//{
	//	// TODO: Rewrite to Class::registerObject()

	//	TypeManager::getInstance()->registerObject<Entity>();
	//	TypeManager::getInstance()->registerObject<Component>();
	//	TypeManager::getInstance()->registerObject<World>();

	//	TypeManager::getInstance()->registerObject<CameraComponent>();
	//	TypeManager::getInstance()->registerObject<CameraYawPitchRollComponent>();
	//	TypeManager::getInstance()->registerObject<CameraFirstPersonComponent>();

	//	TypeManager::getInstance()->registerObject<LogicComponent>();

	//	// Physics
	//	ShapeComponent::registerObject();
	//	BoxShapeComponent::registerObject();
	//	SphereShapeComponent::registerObject();
	//	CylinderShapeComponent::registerObject();
	//	CapsuleShapeComponent::registerObject();
	//	TriangleMeshShapeComponent::registerObject();
	//	RigidBodyComponent::registerObject();
	//	RigidBodyProxyComponent::registerObject();

	//	/*Node::registerObject();
	//	CameraNode::registerObject();

	//	Component::registerObject();
	//	TransformComponent::registerObject();
	//	LogicComponent::registerObject();


	//	WorldComponent::registerObject();
	//	WorldEnvironmentComponent::registerObject();

	//	World::registerObject();
	//	LoadingRoomManager::registerObject();*/
	//}

	// There is more nice looking object registration
	void registerEngineObjects()
	{
		const TypeInfo* engineClasses[] = 
		{
			// base types
			Entity::getStaticTypeInfo(),
			Component::getStaticTypeInfo(),
			World::getStaticTypeInfo(),

			// camera
			CameraComponent::getStaticTypeInfo(),
			CameraYawPitchRollComponent::getStaticTypeInfo(),
			CameraFirstPersonComponent::getStaticTypeInfo(),

			// logic
			LogicComponent::getStaticTypeInfo(),

			// physics
			ShapeComponent::getStaticTypeInfo(),
			BoxShapeComponent::getStaticTypeInfo(),
			SphereShapeComponent::getStaticTypeInfo(),
			CylinderShapeComponent::getStaticTypeInfo(),
			CapsuleShapeComponent::getStaticTypeInfo(),
			TriangleMeshShapeComponent::getStaticTypeInfo(),
			RigidBodyComponent::getStaticTypeInfo(),
			RigidBodyProxyComponent::getStaticTypeInfo()
		};

		// register types
		for (int i = 0; i < sizeof(engineClasses) / sizeof(engineClasses[0]); i++)
			TypeManager::getInstance()->registerType(engineClasses[i]);
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

		//destroyComponentCacheSystem();
	}

	void Engine::loadWorld(const std::string& filename)
	{
		if (ms_world)
		{
			mem_delete(ms_world);
			ms_world = nullptr;
		}

		Core::msg("Engine: Loading world %s", filename.c_str());

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
			Core::error("Engine::loadWorld: Failed to parse world %s.", filename.c_str());
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

		Core::msg("Engine: Creating empty world");

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
		if (m_currentState == m_nextState)
			return;

		m_currentState = m_nextState;
		onStateSwitch();
	}

	void EngineStateManager::onStateSwitch()
	{
		static const char* s_stateNames[(int)EngineState::Count] =
		{
			"None",
			"Running",
			"LoadWorld"
		};

		Core::msg("Engine: Switching to state %s", s_stateNames[(int)m_nextState]);

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
}