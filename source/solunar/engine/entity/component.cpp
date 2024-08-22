#include "enginepch.h"
#include "engine/entity/component.h"

namespace solunar
{

IMPLEMENT_OBJECT(Component, SerializableObject);

BEGIN_PROPERTY_REGISTER(Component)
{
	REGISTER_PROPERTY(Component, PropertyBool, m_isActive);
}
END_PROPERTY_REGISTER(Component)

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

void Component::SetActive(bool active)
{
	m_isActive = active;
}

bool Component::IsActive()
{
	return m_isActive;
}

void Component::OnEntitySet(Entity* entity)
{
	m_entity = entity;
}

void Component::OnWorldSet(World* world)
{
	m_world = world;
}

void Component::OnEntityRemove()
{
	m_entity = nullptr;
}

void Component::SetSerializable(bool value)
{
	m_isSerializable = value;
}

}