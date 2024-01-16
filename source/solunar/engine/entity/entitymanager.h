#ifndef ENGINE_ENTITY_ENTITYMANAGER_H
#define ENGINE_ENTITY_ENTITYMANAGER_H

namespace engine
{

class Entity;

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	Entity* createEntity();
	Entity* createEntityEx(const TypeInfo* typeInfo);

	const std::vector<Entity*>& getEntities() { return m_entities; }

private:
	std::vector<Entity*> m_entities;
};

}

#endif // !ENGINECORE_ENTITY_ENTITYMANAGER_H
