#ifndef ENGINE_ENTITY_WORLD_H
#define ENGINE_ENTITY_WORLD_H

#include "engine/entity/entitymanager.h"

namespace engine
{

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

		Entity* createEntity();
		Entity* createEntityEx(const TypeInfo* typeInfo);

		EntityManager& getEntityManager() { return m_entityManager; }

	private:
		EntityManager m_entityManager;
	};

}

#endif // !ENGINECORE_ENTITY_LEVEL_H
