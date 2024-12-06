#ifndef ENGINE_ENTITY_COMPONENT_H
#define ENGINE_ENTITY_COMPONENT_H

namespace solunar
{

class Entity;
class World;

// Base class for entity component
class Component : public SerializableObject
{
	friend class Entity;
	friend class World;
public:
	DECLARE_OBJECT(Component);
	DECLARE_PROPERTY_REGISTER(Component);
public:
	Component();
	virtual ~Component();

	void SetActive(bool active);
	bool IsActive();

	virtual void OnEntitySet(Entity* entity);
	virtual void OnWorldSet(World* world);

	virtual void OnInit();

	virtual void OnEntityRemove();

	Entity* GetEntity() { return m_entity; }
	World* GetWorld() { return m_world; }

	void SetSerializable(bool value);
	bool IsSerializable() { return m_isSerializable; }

private:
	Entity* m_entity;
	World* m_world;

	bool m_isActive;
	bool m_isSerializable;
};

}

#endif // !COMPONENT_H
