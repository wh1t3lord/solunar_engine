#include "enginepch.h"
#include "engine/entity/component.h"

namespace engine
{

Component::Component() :
	m_entity(nullptr),
	m_world(nullptr),
	m_isActive(true),
	m_isSerializable(true)
{
}

Component::~Component()
{
	m_entity = nullptr;
	m_world = nullptr;
}

void Component::setActive(bool active)
{
	m_isActive = active;
}

bool Component::isActive()
{
	return m_isActive;
}

void Component::onEntitySet(Entity* entity)
{
	m_entity = entity;
}

void Component::onWorldSet(World* world)
{
	m_world = world;
}

void Component::onEntityRemove()
{
	m_entity = nullptr;
}

void Component::setSerializable(bool value)
{
	m_isSerializable = value;
}

}