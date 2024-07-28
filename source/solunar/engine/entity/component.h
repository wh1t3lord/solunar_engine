#ifndef ENGINE_ENTITY_COMPONENT_H
#define ENGINE_ENTITY_COMPONENT_H

namespace engine
{

class Entity;
class World;

// Base class for entity component
class Component : public SerializableObject
{
	friend class Entity;
	friend class World;
public:
	ImplementObject(Component, SerializableObject);
	DeclarePropertyRegister(Component);
public:
	Component();
	virtual ~Component();

	void setActive(bool active);
	bool isActive();

	virtual void onEntitySet(Entity* entity);
	virtual void onWorldSet(World* world);

	virtual void onEntityRemove();

	Entity* getEntity() { return m_entity; }
	World* getWorld() { return m_world; }

	void setSerializable(bool value);
	bool isSerializable() { return m_isSerializable; }

private:
	Entity* m_entity;
	World* m_world;

	bool m_isActive;
	bool m_isSerializable;
};

}

#endif // !COMPONENT_H
