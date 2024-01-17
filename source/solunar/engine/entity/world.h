#ifndef ENGINE_ENTITY_WORLD_H
#define ENGINE_ENTITY_WORLD_H

#include "engine/entity/entitymanager.h"

namespace engine
{

	class PhysicsWorld;

	class World : public SerializableObject
	{
	public:
		ImplementObject(World, SerializableObject);

	public:
		World();
		~World();

		void loadXML(tinyxml2::XMLElement& element) override;
		void saveXML(tinyxml2::XMLElement& element) override;

		void updateLogicWorld();
		void updatePhysicsWorld();

		Entity* createEntity();
		Entity* createEntityEx(const TypeInfo* typeInfo);

		EntityManager& getEntityManager() { return m_entityManager; }

		PhysicsWorld* getPhysicsWorld() { return m_physicsWorld; }

	private:
		EntityManager m_entityManager;

		PhysicsWorld* m_physicsWorld;
	};

}

#endif // !ENGINECORE_ENTITY_LEVEL_H
