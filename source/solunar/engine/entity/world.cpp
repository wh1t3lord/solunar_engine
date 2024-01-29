#include "enginepch.h"
#include "core/timer.h"
#include "core/object/typemanager.h"
#include "engine/entity/entity.h"
#include "engine/entity/entitymanager.h"
#include "engine/entity/world.h"
#include "engine/entity/logiccomponent.h"
#include "engine/physics/physicsworld.h"
#include "engine/physics/rigidbodycomponent.h"
#include "engine/physics/shapescomponent.h"

namespace engine
{
	World::World() :
		m_physicsWorld(nullptr)
	{
		m_physicsWorld = mem_new<PhysicsWorld>();
	}

	World::~World()
	{
		if (m_physicsWorld)
		{
			mem_delete(m_physicsWorld); m_physicsWorld = nullptr;
		}
	}

	void World::loadXML(tinyxml2::XMLElement& element)
	{
		if (strcmp(element.Name(), "World") != 0)
			Core::error("World::loadXML: '%s' is not a world element.", element.Name());

		tinyxml2::XMLElement* entityElement = element.FirstChildElement("Entity");
		while (entityElement)
		{
			const TypeInfo* entityTypeInfo = Entity::getStaticTypeInfo();

			// if classid
			if (const tinyxml2::XMLAttribute* nodeClassId = entityElement->FindAttribute("classId"))
			{
				entityTypeInfo = g_typeManager->getTypeInfoById(nodeClassId->IntValue());
			}
			// if classname
			else if (const tinyxml2::XMLAttribute* nodeClassName = entityElement->FindAttribute("className"))
			{
				entityTypeInfo = g_typeManager->getTypeInfoByName(nodeClassId->Value());
			}

			Entity* entity = createEntityEx(entityTypeInfo);
			entity->loadXML(*entityElement);

			postInitializeEntity(entity);

			entityElement = entityElement->NextSiblingElement("Entity");
		}
	}

	void World::saveXML(tinyxml2::XMLElement& element)
	{
	}

	void World::postInitializeEntity(Entity* entity)
	{
		// initialize physics shapes
		std::vector<ShapeComponent*> shapes = entity->getComponents<ShapeComponent>();
		if (!shapes.empty())
		{
			for (auto it : shapes)
				it->initializeShape();
		}
	}

	void World::update_PreEntityUpdate()
	{
		const std::vector<Entity*>& entities = m_entityManager.getEntities();
		for (auto it : entities)
		{
			it->updateWorldTransform();
			it->transformBBox();
		}
	}

	void World::updateLogicWorld()
	{
		std::vector<Entity*> logicEntities = m_entityManager.getEntitiesWithComponent<LogicComponent>();
		for (auto entity : logicEntities)
		{
			LogicComponent* logicComponent = entity->getComponent<LogicComponent>();
			logicComponent->update(Timer::getInstance()->getDelta());
		}
	}

	void World::updatePhysicsWorld()
	{
		float delta = Timer::getInstance()->getDelta();
		m_physicsWorld->step(delta);

		std::vector<Entity*> physicsEntities = m_entityManager.getEntitiesWithComponent<RigidBodyComponent>();

		for (auto it : physicsEntities)
		{
			RigidBodyComponent* rigidBody = it->getComponent<RigidBodyComponent>();
			if (rigidBody)
			{
				rigidBody->updateBodyTranslationDirty();
				//rigidBody->updateNodeTranslationDirty();
			}
		}
	}

	Entity* World::createEntity()
	{
		Entity* entity = m_entityManager.createEntity();
		entity->onWorldSet(this);
		return entity;
	}

	Entity* World::createEntityEx(const TypeInfo* typeInfo)
	{
		Entity* entity = m_entityManager.createEntityEx(typeInfo);
		entity->onWorldSet(this);
		return entity;
	}

	Entity* World::rayCast(const glm::vec3& rayStart, const glm::vec3& rayEnd)
	{
		Assert2(m_physicsWorld, "Physics world is not initialized for ray casting");

		btDynamicsWorld::ClosestRayResultCallback rayCallback(glmVectorToBt(rayStart), glmVectorToBt(rayEnd));
		m_physicsWorld->getWorld()->rayTest(glmVectorToBt(rayStart), glmVectorToBt(rayEnd), rayCallback);
		if (rayCallback.hasHit())
		{
			// Query entity
			
			RigidBodyComponent* rigidbody = reinterpret_cast<RigidBodyComponent*>(rayCallback.m_collisionObject->getUserPointer());
			Entity* entity = rigidbody->getEntity();
			Assert2(entity, "Critital skill issue, rigid body component assigned to nullptr entity!");
			return entity;
		}

		return nullptr;
	}

	std::vector<Entity*> World::boxCast(const glm::vec3& boxPos, const glm::vec3& boxSize)
	{
		std::vector<Entity*> entities;
		
		glm::vec3 halfSize = boxSize / glm::vec3(2.0f);
		btConvexShape* convexShape = mem_new<btBoxShape>(glmVectorToBt(halfSize));

		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(glmVectorToBt(boxPos));

	//	m_physicsWorld->getWorld()->convexSweepTest(convexShape, transform, )

		//btDynamicsWorld::ClosestConvexResultCallback convexCallback()

		return entities;
	}

}