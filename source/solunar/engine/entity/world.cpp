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
#include <imgui.h>

namespace solunar
{
	IMPLEMENT_OBJECT(World, SerializableObject);

	World::World() :
		m_physicsWorld(nullptr), m_graphicsWorld(nullptr)
	{
		m_physicsWorld = mem_new<PhysicsWorld>();
		m_graphicsWorld = mem_new<GraphicsWorld>();
	}

	World::~World()
	{
		// Delete all entities before physics world destruction
		m_entityManager.Destroy();

		if (m_physicsWorld)
		{
			mem_delete(m_physicsWorld); m_physicsWorld = nullptr;
		}

		if (m_graphicsWorld)
		{
			mem_delete(m_graphicsWorld); m_graphicsWorld = nullptr;
		}
	}

	void World::LoadXML(tinyxml2::XMLElement& element)
	{
		if (strcmp(element.Name(), "World") != 0)
			Core::Error("World::LoadXML: '%s' is not a world element.", element.Name());

		tinyxml2::XMLElement* entityElement = element.FirstChildElement("Entity");
		while (entityElement)
		{
			const TypeInfo* entityTypeInfo = Entity::GetStaticTypeInfo();

			// if classid
			if (const tinyxml2::XMLAttribute* nodeClassId = entityElement->FindAttribute("classId"))
			{
				entityTypeInfo = g_typeManager->GetTypeInfoById(nodeClassId->IntValue());
			}
			// if classname
			else if (const tinyxml2::XMLAttribute* nodeClassName = entityElement->FindAttribute("className"))
			{
				entityTypeInfo = g_typeManager->GetTypeInfoByName(nodeClassId->Value());
			}

			Entity* entity = CreateEntityEx(entityTypeInfo);
			entity->LoadXML(*entityElement);

			PostInitializeEntity(entity);

			entityElement = entityElement->NextSiblingElement("Entity");
		}
	}

	void World::SaveXML(tinyxml2::XMLElement& element)
	{
	}

	void World::PostInitializeEntity(Entity* entity)
	{
		// Initialize physics shapes
		std::vector<ShapeComponent*> shapes = entity->GetComponents<ShapeComponent>();
		if (!shapes.empty())
		{
			for (auto it : shapes)
				it->InitializeShape();
		}
	}

	void World::Update_PreEntityUpdate()
	{
		const std::vector<Entity*>& entities = m_entityManager.GetEntities();
		for (auto it : entities)
		{
			it->UpdateWorldTransform();
			it->TransformBBox();
		}
	}

	void World::Update_LogicEntity()
	{
		std::vector<Entity*> logicEntities = m_entityManager.GetEntitiesWithComponent<LogicComponent>();
		for (auto entity : logicEntities)
		{
			LogicComponent* logicComponent = entity->GetComponent<LogicComponent>();
			logicComponent->Update(Timer::GetInstance()->GetDelta());
		}
	}

	void World::Update_PhysicsEntity()
	{
		float delta = Timer::GetInstance()->GetDelta();
		m_physicsWorld->Step(delta);

		std::vector<Entity*> physicsEntities = m_entityManager.GetEntitiesWithComponent<RigidBodyComponent>();

		for (auto it : physicsEntities)
		{
			RigidBodyComponent* rigidBody = it->GetComponent<RigidBodyComponent>();
			if (rigidBody)
			{
				rigidBody->UpdateBodyTranslationDirty();
				//rigidBody->updateNodeTranslationDirty();
			}
		}
	}

	Entity* World::CreateEntity()
	{
		Entity* entity = m_entityManager.CreateEntity();
		entity->OnWorldSet(this);
		return entity;
	}

	Entity* World::CreateEntityEx(const TypeInfo* typeInfo)
	{
		Entity* entity = m_entityManager.CreateEntityEx(typeInfo);
		entity->OnWorldSet(this);
		return entity;
	}

	void World::TogglePhysicsDebugDraw()
	{
		m_physicsWorld->ToggleDebugDraw();
	}

	bool World::RayCast(RayCastResult& rayResult, const glm::vec3& rayStart, const glm::vec3& rayEnd, const int collisionFilter /* =-1*/)
	{
		Assert2(m_physicsWorld, "Physics world is not initialized for ray casting");

		btDynamicsWorld::ClosestRayResultCallback rayCallback(glmVectorToBt(rayStart), glmVectorToBt(rayEnd));
		
		if (collisionFilter == -1)
			rayCallback.m_collisionFilterMask = kCollisionFilterAllMask ^ kPhysicsFilter_AllAux;
		else
		{
			rayCallback.m_collisionFilterGroup = btBroadphaseProxy::SensorTrigger;
			rayCallback.m_collisionFilterMask = PhysicsFilter_Player;
		}

		m_physicsWorld->GetWorld()->rayTest(glmVectorToBt(rayStart), glmVectorToBt(rayEnd), rayCallback);
		if (rayCallback.hasHit())
		{
			// Query entity
			
			RigidBodyComponent* rigidbody = reinterpret_cast<RigidBodyComponent*>(rayCallback.m_collisionObject->getUserPointer());
			Assert2(rigidbody, "Critital skill issue, rigid body component is nullptr!");

			Entity* entity = rigidbody->GetEntity();
			Assert2(entity, "Critital skill issue, rigid body component assigned to nullptr entity!");
			
			rayResult.m_entity = entity;
			rayResult.m_hitPosition = btVectorToGlm(rayCallback.m_hitPointWorld);
			rayResult.m_hitNormal = btVectorToGlm(rayCallback.m_hitNormalWorld);
			return true;
		}

		return false;
	}

	std::vector<Entity*> World::BoxCast(const glm::vec3& boxPos, const glm::vec3& boxSize)
	{
		std::vector<Entity*> entities;
		
		glm::vec3 halfSize = boxSize / glm::vec3(2.0f);
		
		//btConvexShape* convexShape = mem_new<btBoxShape>(glmVectorToBt(halfSize));

		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(glmVectorToBt(boxPos));

	//	m_physicsWorld->getWorld()->convexSweepTest(convexShape, transform, )

		//btDynamicsWorld::ClosestConvexResultCallback convexCallback()

		return entities;
	}

}