#include "enginepch.h"
#include "engine/entity/entity.h"
#include "engine/entity/entitymanager.h"

namespace engine
{

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
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

}