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

		void postInitializeEntity(Entity* entity);

		void update_PreEntityUpdate();
		void updateLogicWorld();
		void updatePhysicsWorld();

		Entity* createEntity();
		Entity* createEntityEx(const TypeInfo* typeInfo);

		EntityManager& getEntityManager() { return m_entityManager; }

		PhysicsWorld* getPhysicsWorld() { return m_physicsWorld; }

		// Casting functions

		Entity* rayCast(const glm::vec3& rayStart, const glm::vec3& rayEnd);
		std::vector<Entity*> boxCast(const glm::vec3& boxPos, const glm::vec3& boxSize);

	private:
		EntityManager m_entityManager;

		PhysicsWorld* m_physicsWorld;
	};

}

#endif // !ENGINECORE_ENTITY_LEVEL_H
