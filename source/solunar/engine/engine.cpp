#include "enginepch.h"
#include "core/file/contentmanager.h"
#include "engine/engine.h"
#include "engine/entity/entity.h"
#include "engine/entity/component.h"
#include "engine/entity/world.h"
#include "engine/entity/cameracomponent.h"
#include "engine/entity/logiccomponent.h"

namespace engine
{
	namespace
	{
		// IGameInterface* g_gameInterface = nullptr;

		void registerEngineObjects()
		{
			TypeManager::getInstance()->registerObject<Entity>();
			TypeManager::getInstance()->registerObject<Component>();
			TypeManager::getInstance()->registerObject<World>();

			TypeManager::getInstance()->registerObject<CameraComponent>();
			TypeManager::getInstance()->registerObject<CameraYawPitchRollComponent>();
			TypeManager::getInstance()->registerObject<CameraFirstPersonComponent>();

			TypeManager::getInstance()->registerObject<LogicComponent>();

			/*Node::registerObject();
			CameraNode::registerObject();

			Component::registerObject();
			TransformComponent::registerObject();
			LogicComponent::registerObject();


			WorldComponent::registerObject();
			WorldEnvironmentComponent::registerObject();

			World::registerObject();
			LoadingRoomManager::registerObject();

			ShapeComponent::registerObject();
			BoxShapeComponent::registerObject();
			SphereShapeComponent::registerObject();
			CylinderShapeComponent::registerObject();
			CapsuleShapeComponent::registerObject();
			RigidBodyComponent::registerObject();*/
		}

	}

	World* Engine::ms_world = nullptr;

	void Engine::init()
	{
		Core::msg("Initializing engine");

		registerEngineObjects();

		//ScriptManager::getInstance()->init();

		// initalize console
		//g_console->init();

		//WorldManager::init();
	}

	void Engine::shutdown()
	{		
		//WorldManager::shutdown();

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

		World* world = g_typeManager->createObject<World>();
		ms_world = world;
	}

	void Engine::update()
	{
		//OPTICK_EVENT("Engine::update");

		//g_eventManager.update();

		//std::shared_ptr<World> world = WorldManager::getActiveWorld();
		//if (world)
		//{
		//	world->updatePhysicsWorld();
		//	world->updateLogicWorld();
		//}
	}
	void Engine::setGameInterface(IGameInterface* gameInterface)
	{
	}

	IGameInterface* Engine::getGameInterface()
	{
		return nullptr;
	}
}