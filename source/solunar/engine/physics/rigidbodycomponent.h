#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include "engine/physics/bullet_private.h"

namespace engine {
	
	class PhysicsManager;
	class PhysicsWorld;

	//! Rigid body component
	class RigidBodyComponent : public Component
	{
		ImplementObject(RigidBodyComponent, Component);
	public:
		RigidBodyComponent();
		~RigidBodyComponent();
		
		static void registerObject();
		
		void onEntitySet(Entity* entity) override;
		void onWorldSet(World* world) override;

		void onEntityRemove() override;
		
		void loadXML(tinyxml2::XMLElement& element) override;
		void saveXML(tinyxml2::XMLElement& element) override;

		void updateEntityTranslationDirty();
		void updateBodyTranslationDirty();

		// Setter's and getter's
		const float getMass() { return m_mass; } 
		void setMass(float value) { m_mass = value; }

		btRigidBody* getSDKBody() { return m_rigidBody; }
		btCompoundShape* getCompoundShape() { return m_compoundShape; }

		//void getWorldTransform(btTransform& worldTrans) const override;
		//void setWorldTransform(const btTransform& worldTrans) override;

		void disableCollide();
		void enableCollide();

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
	
}

#endif // !RIGIDBODYCOMPONENT_H
