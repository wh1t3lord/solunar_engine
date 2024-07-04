#include "enginepch.h"
#include "engine/entity/entity.h"
#include "engine/physics/physicsworld.h"
#include "engine/physics/physicsdebugdraw.h"
#include "engine/physics/rigidbodycomponent.h"

#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

namespace engine
{
	static void internalTickCallback(btDynamicsWorld* world, btScalar timeStep)
	{
		Assert(world);
		Assert(world->getWorldUserInfo());
		static_cast<PhysicsWorld*>(world->getWorldUserInfo())->internalTick();
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
		m_world->setInternalTickCallback(internalTickCallback, this);
		m_world->setDebugDrawer(&g_physicsDebugDraw);

		m_accumulatedTime = 0.0f;
		m_stepTime = (1.0f / 60.0f); // 30 fps

		// set the standart gravity
		//m_world->setGravity(btVector3(0, 0, 0));
		getWorld()->setGravity(btVector3(0.0, -15.0, 0.0));
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

	void PhysicsWorld::addRigidBody(RigidBodyComponent* body)
	{
		Assert2(body->getSDKBody(), "RigidBodyComponent is not properly initialized");
		m_world->addRigidBody(body->getSDKBody());
	}

	void PhysicsWorld::step(float delta)
	{
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

		//m_debugDraw = true;
		if (m_debugDraw)
		{
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

	void PhysicsWorld::toggleDebugDraw()
	{
		m_debugDraw = !m_debugDraw;
	}

	void PhysicsWorld::internalTick()
	{
#if 0
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

					// player is obA
					if (btGhostObject* ghostObject = btGhostObject::upcast(obA))
					{
						if (obB)
						{
							RigidBodyComponent* rb = static_cast<RigidBodyComponent*>(obB->getUserPointer());
							if (TriggerComponent* trigger = rb->getNode()->getComponentByType<TriggerComponent>())
							{
								ASSERT(ghostObject->getUserPointer());// no node instance sadly :(

								Node* node = static_cast<Node*>(ghostObject->getUserPointer());
								trigger->onCollidePlayer(rb, node);
							}
						}
					}
					// player is obB
					else if (btGhostObject* ghostObject = btGhostObject::upcast(obB))
					{
						if (obA)
						{
							RigidBodyComponent* rb = static_cast<RigidBodyComponent*>(obA->getUserPointer());
							if (TriggerComponent* trigger = rb->getNode()->getComponentByType<TriggerComponent>())
							{
								ASSERT(ghostObject->getUserPointer());// no node instance sadly :(

								Node* node = static_cast<Node*>(ghostObject->getUserPointer());
								trigger->onCollidePlayer(rb, node);
							}
						}
					}
					else
					{
						RigidBodyComponent* rbA = static_cast<RigidBodyComponent*>(obA->getUserPointer());
						RigidBodyComponent* rbB = static_cast<RigidBodyComponent*>(obB->getUserPointer());

						if (!rbA || !rbB)
							continue;

						ASSERT(rbA);
						ASSERT(rbB);

						if (TriggerComponent* trigger = rbA->getNode()->getComponentByType<TriggerComponent>())
						{
							trigger->onCollide(rbA, rbB);
						}
						else if (TriggerComponent* trigger = rbB->getNode()->getComponentByType<TriggerComponent>())
						{
							trigger->onCollide(rbB, rbA);
						}
					}
				}
			}
		}
#endif
	}
}
