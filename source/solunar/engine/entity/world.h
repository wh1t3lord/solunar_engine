#ifndef ENGINE_ENTITY_WORLD_H
#define ENGINE_ENTITY_WORLD_H

#include "engine/entity/entitymanager.h"
#include "graphics/graphicsworld.h"

namespace engine
{
	class PhysicsWorld;

	struct RayCastResult
	{
		Entity* m_entity;
		glm::vec3 m_hitPosition;
		glm::vec3 m_hitNormal;
	};

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

		GraphicsWorld* getGraphicsWorld() { return m_graphicsWorld; }
		PhysicsWorld* getPhysicsWorld() { return m_physicsWorld; }

		void togglePhysicsDebugDraw();

		// Casting functions

		bool rayCast(RayCastResult& rayResult, const glm::vec3& rayStart, const glm::vec3& rayEnd);
		std::vector<Entity*> boxCast(const glm::vec3& boxPos, const glm::vec3& boxSize);

	private:
		EntityManager m_entityManager;

		GraphicsWorld* m_graphicsWorld;
		PhysicsWorld* m_physicsWorld;
	};

}

#endif // !ENGINECORE_ENTITY_LEVEL_H
