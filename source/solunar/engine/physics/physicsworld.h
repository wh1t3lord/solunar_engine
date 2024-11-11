#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H

#include "engine/physics/bullet_private.h"
#include "engine/physics/physicsdebugdraw.h"

#include <vector>

namespace solunar
{
	class RigidBodyComponent;

	class PhysicsWorld
	{
	public:
		PhysicsWorld();
		~PhysicsWorld();

		void AddRigidBody(RigidBodyComponent* body);
		void RemoveRigidBody(RigidBodyComponent* body);

		const std::vector<RigidBodyComponent*>& GetRigidBodies();

		void Step(float delta);

		btDynamicsWorld* GetWorld() { return m_world; }

		float GetFixedTimeStep() { return m_stepTime; }

		void ToggleDebugDraw();

		void InternalTick();

		void DebugDrawTriggers();

	private:
		btDefaultCollisionConfiguration* m_collisionConfiguration;
		btCollisionDispatcher* m_dispatcher;
		btBroadphaseInterface* m_overlappingPairCache;
		btSequentialImpulseConstraintSolver* m_solver;
		btDiscreteDynamicsWorld* m_world;
		btGhostPairCallback* m_btGhostPairCallback;

		PhysicsDebugDraw m_physicsDebugDraw;

		std::vector<RigidBodyComponent*> m_rigidbodies;

		float m_accumulatedTime;
		float m_stepTime;

		bool m_debugDraw;
	};
}

#endif // !PHYSICSWORLD_H
