#include "enginepch.h"
#include "engine/engine.h"
#include "engine/camera.h"
#include "engine/entity/world.h"
#include "engine/entity/entity.h"
#include "engine/physics/physicsworld.h"
#include "engine/physics/physicsdebugdraw.h"
#include "engine/physics/rigidbodycomponent.h"
#include "engine/physics/triggercomponent.h"

// #TODO: REMOVE GRAPHICS FROM ENGINE
#include "graphics/ifontmanager.h"

#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

namespace solunar
{
	static void InternalTickCallback(btDynamicsWorld* world, btScalar timeStep)
	{
		Assert(world);
		Assert(world->getWorldUserInfo());
		static_cast<PhysicsWorld*>(world->getWorldUserInfo())->InternalTick();
	}

	PhysicsWorld::PhysicsWorld()
	{
		m_debugDraw = false;

		m_collisionConfiguration	= mem_new<btDefaultCollisionConfiguration>();
		m_dispatcher				= mem_new<btCollisionDispatcher>(m_collisionConfiguration);
		m_overlappingPairCache		= mem_new<btDbvtBroadphase>();
		m_solver					= mem_new<btSequentialImpulseConstraintSolver>();
		m_world						= mem_new<btDiscreteDynamicsWorld>(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);
		m_btGhostPairCallback		= mem_new<btGhostPairCallback>();

		m_world->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(m_btGhostPairCallback);
		m_world->setInternalTickCallback(InternalTickCallback, this);
		m_world->setDebugDrawer(&g_physicsDebugDraw);

		m_accumulatedTime = 0.0f;
		m_stepTime = (1.0f / 60.0f); // 30 fps

		// set the standart gravity
		//m_world->setGravity(btVector3(0, 0, 0));
		//getWorld()->setGravity(btVector3(0.0, -15.0, 0.0));
	}

	PhysicsWorld::~PhysicsWorld()
	{
		m_world->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(nullptr);

		// delete ghost pair callback
		mem_delete(m_btGhostPairCallback);

		// delete dynamics world
		mem_delete(m_world);

		// delete solver
		mem_delete(m_solver);

		// delete broadphase
		mem_delete(m_overlappingPairCache);

		// delete dispatcher
		mem_delete(m_dispatcher);

		// delete collision configuration
		mem_delete(m_collisionConfiguration);

	}

	void PhysicsWorld::AddRigidBody(RigidBodyComponent* body)
	{
		Assert2(body->GetSDKBody(), "RigidBodyComponent is not properly initialized");
		m_world->addRigidBody(body->GetSDKBody());
	}

	void PhysicsWorld::RemoveRigidBody(RigidBodyComponent* body)
	{
		Assert2(body->GetSDKBody(), "RigidBodyComponent is not properly initialized");
		m_world->removeRigidBody(body->GetSDKBody());
	}

	void PhysicsWorld::Step(float delta)
	{
		m_world->stepSimulation(delta, 12, m_stepTime);
#if 0
		if (delta < 0.01f)
		{
			m_accumulatedTime += delta;
			if (m_accumulatedTime > m_stepTime)
			{
				m_world->stepSimulation(m_stepTime);
				m_accumulatedTime -= m_stepTime;
			}
		}
		else
		{
			m_world->stepSimulation(delta);
		}
#endif

		//m_debugDraw = true;
		if (m_debugDraw)
		{
			//DebugDrawTriggers();

			int debugDrawMode = 0;
			debugDrawMode |= btIDebugDraw::DBG_DrawAabb;
			debugDrawMode |= btIDebugDraw::DBG_DrawWireframe;
			// debugDrawMode |= btIDebugDraw::DBG_DrawContactPoints;
			g_physicsDebugDraw.setDebugMode(debugDrawMode);

			m_world->debugDrawWorld();
		}

		/*if (m_world)
			m_world->stepSimulation(1 / 60.0f);
		
		int debugDrawMode = 0;
		debugDrawMode |= btIDebugDraw::DBG_DrawAabb;
		debugDrawMode |= btIDebugDraw::DBG_DrawWireframe;
		debugDrawMode |= btIDebugDraw::DBG_DrawContactPoints;
		g_physicsDebugDraw.setDebugMode(debugDrawMode);
		
		m_world->debugDrawWorld();*/
	}

	void PhysicsWorld::ToggleDebugDraw()
	{
		m_debugDraw = !m_debugDraw;
	}

	void PhysicsWorld::InternalTick()
	{
		int numManifolds = m_world->getDispatcher()->getNumManifolds();
		for (int i = 0; i < numManifolds; i++)
		{
			btPersistentManifold* contactManifold = m_world->getDispatcher()->getManifoldByIndexInternal(i);
			btCollisionObject* obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
			btCollisionObject* obB = const_cast<btCollisionObject*>(contactManifold->getBody1());

			int numContacts = contactManifold->getNumContacts();
			for (int j = 0; j < numContacts; j++)
			{
				btManifoldPoint& pt = contactManifold->getContactPoint(j);
				if (pt.getDistance() < 0.f)
				{
					const btVector3& ptA = pt.getPositionWorldOnA();
					const btVector3& ptB = pt.getPositionWorldOnB();
					const btVector3& normalOnB = pt.m_normalWorldOnB;

					RigidBodyComponent* rbA = static_cast<RigidBodyComponent*>(obA->getUserPointer());
					RigidBodyComponent* rbB = static_cast<RigidBodyComponent*>(obB->getUserPointer());

					if (!rbA || !rbB)
						continue;

					Assert(rbA);
					Assert(rbB);

					if (TriggerComponent* trigger = rbA->GetEntity()->GetComponent<TriggerComponent>())
					{
						trigger->OnCollide(rbA, rbB);
					}
					else if (TriggerComponent* trigger = rbB->GetEntity()->GetComponent<TriggerComponent>())
					{
						trigger->OnCollide(rbB, rbA);
					}
				}
			}
		}
	}

	void PhysicsWorld::DebugDrawTriggers()
	{
		if (Engine::ms_world)
		{
			std::vector<Entity*> triggers = Engine::ms_world->GetEntityManager().GetEntitiesWithComponent<TriggerComponent>();

			int numTriggers = triggers.size();
			for (int i = 0; i < numTriggers; i++)
			{
				RigidBodyComponent* body = triggers[i]->GetComponent<RigidBodyComponent>();
				btCompoundShape* compoundShape = body->GetCompoundShape();

				btVector3 aabbMin;
				btVector3 aabbMax;
				compoundShape->getAabb(body->GetSDKBody()->getWorldTransform(), aabbMin, aabbMax);

				const btVector3 color = btVector3(1.0f, 0.6f, 0.0f);
				g_physicsDebugDraw.drawBox(aabbMin, aabbMax, color);

				std::string entityName = "Trigger #" + std::to_string(i);
			}
		}
	}
}
