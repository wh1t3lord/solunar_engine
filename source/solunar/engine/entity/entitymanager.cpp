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
	Destroy();
}

void EntityManager::Destroy()
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

Entity* EntityManager::CreateEntity()
{
	Entity* entity = g_typeManager->CreateObject<Entity>();
	m_entities.push_back(entity);
	return entity;
}

Entity* EntityManager::CreateEntityEx(const TypeInfo* typeInfo)
{
	Entity* entity = (Entity*)g_typeManager->CreateObjectByTypeInfo(typeInfo);
	m_entities.push_back(entity);
	return entity;
}

std::vector<Entity*> EntityManager::GetEntitiesWithComponentTypeInfo(const TypeInfo* typeInfo)
{
	std::vector<Entity*> entities;

	for (auto entity : m_entities)
	{
		if (entity->GetComponentByTypeInfo(typeInfo))
			entities.push_back(entity);
	}

	return entities;
}

}