#include "enginepch.h"
#include "core/timer.h"
#include "core/object/typemanager.h"
#include "engine/entity/entity.h"
#include "engine/entity/entitymanager.h"
#include "engine/entity/world.h"
#include "engine/entity/logiccomponent.h"

namespace engine
{
	World::World()
	{
	}

	World::~World()
	{
	}

	void World::loadXML(tinyxml2::XMLElement& element)
	{
		if (strcmp(element.Name(), "World") != 0)
			Core::error("World::loadXML: '%s' is not a world element.", element.Name());

		tinyxml2::XMLElement* entityElement = element.FirstChildElement("Entity");
		while (entityElement)
		{
			const TypeInfo* entityTypeInfo = Entity::getStaticTypeInfo();

			// if classid
			if (const tinyxml2::XMLAttribute* nodeClassId = entityElement->FindAttribute("classId"))
			{
				entityTypeInfo = g_typeManager->getTypeInfoById(nodeClassId->IntValue());
			}
			// if classname
			else if (const tinyxml2::XMLAttribute* nodeClassName = entityElement->FindAttribute("className"))
			{
				entityTypeInfo = g_typeManager->getTypeInfoByName(nodeClassId->Value());
			}

			Entity* entity = createEntityEx(entityTypeInfo);
			entity->loadXML(*entityElement);

			entityElement = entityElement->NextSiblingElement("Entity");
		}

	}

	void World::saveXML(tinyxml2::XMLElement& element)
	{
	}

	void World::updateLogicWorld()
	{
		std::vector<Entity*> logicEntities = m_entityManager.getEntitiesWithComponent<LogicComponent>();
		for (auto entity : logicEntities)
		{
			LogicComponent* logicComponent = entity->getComponent<LogicComponent>();
			logicComponent->update(Timer::getInstance()->getDelta());
		}
	}

	Entity* World::createEntity()
	{
		Entity* entity = m_entityManager.createEntity();
		entity->onWorldSet(this);
		return entity;
	}

	Entity* World::createEntityEx(const TypeInfo* typeInfo)
	{
		Entity* entity = m_entityManager.createEntityEx(typeInfo);
		entity->onWorldSet(this);
		return entity;
	}

}