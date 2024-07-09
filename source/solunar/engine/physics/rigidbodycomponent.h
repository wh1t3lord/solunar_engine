#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include "engine/physics/bullet_private.h"

namespace engine {
	
	class PhysicsManager;
	class PhysicsWorld;
	class ShapeComponent;

	//! Rigid body component
	class RigidBodyComponent : public Component
	{
		ImplementObject(RigidBodyComponent, Component);
	public:
		RigidBodyComponent();
		~RigidBodyComponent();
		
		static void registerObject();
		
		virtual void onEntitySet(Entity* entity) override;
		virtual void onWorldSet(World* world) override;

		virtual void onEntityRemove() override;
		
		void loadXML(tinyxml2::XMLElement& element) override;
		void saveXML(tinyxml2::XMLElement& element) override;

		void updateEntityTranslationDirty();
		void updateBodyTranslationDirty();

		// Setter's and getter's
		const float getMass() { return m_mass; } 
		void setMass(float value) { m_mass = value; }

		btRigidBody* getSDKBody() { return m_rigidBody; }
		btCompoundShape* getCompoundShape() { return m_compoundShape; }
		PhysicsWorld* getPhysicsWorld() { return m_physicsWorld; }

		//void getWorldTransform(btTransform& worldTrans) const override;
		//void setWorldTransform(const btTransform& worldTrans) override;

		void disableCollide();
		void enableCollide();

		void attachShape(ShapeComponent* shape);
		void dettachShape(ShapeComponent* shape);

		void applyImpulse(const glm::vec3& impulse);

		void setLinearVelocity(const glm::vec3& velocity);

	private:
		void createBody();
		void destroyBody();
		void updateBodyProperties();
	private:
		btRigidBody* m_rigidBody;
		btCompoundShape* m_compoundShape;

		PhysicsWorld* m_physicsWorld;

		// Body mass
		float m_mass;
		bool m_isEnable;
		bool m_static;
		bool m_isKinematic;
		bool m_inWorld;
	};

	//! Hack due support btCharacterController in the physics system.
	class RigidBodyProxyComponent : public RigidBodyComponent
	{
		ImplementObject(RigidBodyProxyComponent, RigidBodyComponent);
	public:
		RigidBodyProxyComponent();
		~RigidBodyProxyComponent();

		static void registerObject();

		void onEntitySet(Entity* entity) override;
		void onWorldSet(World* world) override;

		void onEntityRemove() override;

		void createPlayerController();
		void update(float dt);
		void setDirection(const glm::vec3& direction);
		void setPositionForce(const glm::vec3& position);

		btKinematicCharacterController* getCharacterController() { return m_characterController; }
		btGhostObject* getGhostObject() { return m_ghostObject; }

	private:
		btKinematicCharacterController* m_characterController;
		btPairCachingGhostObject* m_ghostObject;
		btGhostPairCallback* m_ghostCallback;
		btConvexShape* m_capsuleShape;
	};
}

#endif // !RIGIDBODYCOMPONENT_H
