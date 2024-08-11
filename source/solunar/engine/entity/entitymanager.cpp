#include "enginepch.h"
#include "engine/entity/entity.h"
#include "engine/entity/entitymanager.h"

namespace solunar
{

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
	deleteEntities();
}

void EntityManager::deleteEntities()
{
	if (!m_entities.empty())
	{
		for (auto entity : m_entities)
		{
			if (entity)
			{
				mem_delete(entity);
			}
		}

		m_entities.clear();
	}
}

Entity* EntityManager::createEntity()
{
	Entity* entity = g_typeManager->createObject<Entity>();
	m_entities.push_back(entity);
	return entity;
}

Entity* EntityManager::createEntityEx(const TypeInfo* typeInfo)
{
	Entity* entity = (Entity*)g_typeManager->createObjectByTypeInfo(typeInfo);
	m_entities.push_back(entity);
	return entity;
}

std::vector<Entity*> EntityManager::getEntitiesWithComponentTypeInfo(const TypeInfo* typeInfo)
{
	std::vector<Entity*> entities;

	for (auto entity : m_entities)
	{
		if (entity->getComponentByTypeInfo(typeInfo))
			entities.push_back(entity);
	}

	return entities;
}

}