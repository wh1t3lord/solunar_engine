#ifndef ENGINE_ENTITY_ENTITYMANAGER_H
#define ENGINE_ENTITY_ENTITYMANAGER_H

namespace engine
{

class Entity;

class ENGINE_API EntityManager
{
public:
	EntityManager();
	~EntityManager();

	void deleteEntities();

	Entity* createEntity();
	Entity* createEntityEx(const TypeInfo* typeInfo);

	const std::vector<Entity*>& getEntities() { return m_entities; }

	std::vector<Entity*> getEntitiesWithComponentTypeInfo(const TypeInfo* typeInfo);

	template <typename T>
	std::vector<Entity*> getEntitiesWithComponent();

private:
	std::vector<Entity*> m_entities;
};

template<typename T>
inline std::vector<Entity*> EntityManager::getEntitiesWithComponent()
{
	return getEntitiesWithComponentTypeInfo(T::getStaticTypeInfo());
}

}

#endif // !ENGINECORE_ENTITY_ENTITYMANAGER_H
