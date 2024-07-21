#include "enginepch.h"
#include "engine/physics/rigidbodycomponent.h"
#include "engine/physics/physicsworld.h"
#include "engine/physics/shapescomponent.h"

#include "engine/entity/world.h"

namespace engine {

	RigidBodyComponent::RigidBodyComponent()
	{
		m_rigidBody = nullptr;
		m_compoundShape = nullptr;
		m_physicsWorld = nullptr;

		m_mass = 0.0f;
		m_isEnable = 0.0f;
		m_static = false;
		m_isKinematic = false;
		m_inWorld = false;
	}

	RigidBodyComponent::~RigidBodyComponent()
	{
		m_physicsWorld = nullptr;
		m_compoundShape = nullptr;
		m_rigidBody = nullptr;
	}

	void RigidBodyComponent::registerObject()
	{
		g_typeManager->registerObject<RigidBodyComponent>();
	}

	void RigidBodyComponent::onEntitySet(Entity* entity)
	{
		Component::onEntitySet(entity);

		if (m_physicsWorld && !m_rigidBody)
			createBody();
	}

	void RigidBodyComponent::onWorldSet(World* world)
	{
		Component::onWorldSet(world);
		m_physicsWorld = world->getPhysicsWorld();
	}

	void RigidBodyComponent::onEntityRemove()
	{
		destroyBody();
		Component::onEntityRemove();
	}

	void RigidBodyComponent::loadXML(tinyxml2::XMLElement& element)
	{
		tinyxml2::XMLElement* staticElement = element.FirstChildElement("Static");
		if (staticElement)
		{
			staticElement->QueryBoolAttribute("value", &m_static);
		}

		createBody();
		updateBodyTranslationDirty();
	}

	void RigidBodyComponent::saveXML(tinyxml2::XMLElement& element)
	{
		tinyxml2::XMLElement* enableElement = element.InsertNewChildElement("Enable");
		enableElement->SetAttribute("value", m_isEnable);

		tinyxml2::XMLElement* massElement = element.InsertNewChildElement("Mass");
		massElement->SetAttribute("value", m_mass);

		tinyxml2::XMLElement* staticElement = element.InsertNewChildElement("Static");
		staticElement->SetAttribute("value", m_static);

		tinyxml2::XMLElement* kinematicElement = element.InsertNewChildElement("Kinematic");
		kinematicElement->SetAttribute("value", m_isKinematic);
	}

	void RigidBodyComponent::updateEntityTranslationDirty()
	{
		btTransform trans = m_rigidBody->getWorldTransform();
		getEntity()->setPosition(btVectorToGlm(trans.getOrigin()));

		// calculate rotation based on euler angles
		float roll, pitch, yaw;
		trans.getRotation().getEulerZYX(yaw, pitch, roll);
		getEntity()->setEulerRotation(glm::vec3(roll, pitch, yaw));
	}

	void RigidBodyComponent::updateBodyTranslationDirty()
	{
		if (!m_rigidBody)
			return;

		btTransform trans;
		trans.setIdentity();

		// origin
		trans.setOrigin(glmVectorToBt(getEntity()->getPosition()));

		// rotation
		glm::quat entityRot = getEntity()->getRotation();
		btQuaternion bodyRot;
		bodyRot.setX(entityRot.x);
		bodyRot.setY(entityRot.y);
		bodyRot.setZ(entityRot.z);
		bodyRot.setW(entityRot.w);

		// set transformation to body
		m_rigidBody->setWorldTransform(trans);
	}

#if 0
	void RigidBodyComponent::getWorldTransform(btTransform& worldTrans) const
	{
		//throw std::logic_error("The method or operation is not implemented.");
	}

	void RigidBodyComponent::setWorldTransform(const btTransform& worldTrans)
	{
		//throw std::logic_error("The method or operation is not implemented.");

		updateBodyTranslationDirty();
	}
#endif

	void RigidBodyComponent::createBody()
	{
		Assert(m_physicsWorld);

		m_compoundShape = mem_new<btCompoundShape>();

		btVector3 localInertia(0.0f, 0.0f, 0.0f);
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCInfo(m_mass, nullptr, m_compoundShape, localInertia);
		m_rigidBody = mem_new<btRigidBody>(rigidBodyCInfo);
		
		// TODO: TO THINK
		// It is right to assing entity instance to user pointer or entity should have multiply rigid bodies component ???
		m_rigidBody->setUserPointer(this);

		int bodyCollisionFlags = m_rigidBody->getCollisionFlags();

		if (m_static)
			bodyCollisionFlags |= btRigidBody::CF_STATIC_OBJECT;
		else
			bodyCollisionFlags &= ~btRigidBody::CF_STATIC_OBJECT;

		m_rigidBody->setCollisionFlags(bodyCollisionFlags);

		m_physicsWorld->addRigidBody(this);
		m_inWorld = m_rigidBody->isInWorld();
	}

	void RigidBodyComponent::destroyBody()
	{
		if (m_physicsWorld)
		{
			btDynamicsWorld* physicsWorld = m_physicsWorld->getWorld();
			physicsWorld->removeRigidBody(m_rigidBody);
		}
		else
		{
			Core::msg("RigidBodyComponent::destroyBody: Can't remove body. Physics world is already destroyed.");
			Core::msg("Entity=0x%p", getEntity());
		}
		
		// remove user pointer from this component
		m_rigidBody->setUserPointer(nullptr);

		mem_delete(m_rigidBody);
		m_rigidBody = nullptr;

		mem_delete(m_compoundShape);
		m_compoundShape = nullptr;
	}

	void RigidBodyComponent::updateBodyProperties()
	{
	}

	void RigidBodyComponent::disableCollide()
	{
		m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}

	void RigidBodyComponent::enableCollide()
	{
		m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}

	void RigidBodyComponent::attachShape(ShapeComponent* shape)
	{
		shape->initializeShape();
	}

	void RigidBodyComponent::dettachShape(ShapeComponent* shape)
	{
		m_compoundShape->removeChildShape(shape->getSdkShape());
	}

	void RigidBodyComponent::applyImpulse(const glm::vec3& impulse)
	{
		m_rigidBody->applyCentralImpulse(glmVectorToBt(impulse));
	}

	void RigidBodyComponent::setLinearVelocity(const glm::vec3& velocity)
	{
		m_rigidBody->setLinearVelocity(glmVectorToBt(velocity));
	}

	/////////////////////////////////////////////////////////////////

	RigidBodyProxyComponent::RigidBodyProxyComponent() :
		m_characterController(nullptr),
		m_ghostObject(nullptr),
		m_ghostCallback(nullptr),
		m_capsuleShape(nullptr)
	{
	}

	RigidBodyProxyComponent::~RigidBodyProxyComponent()
	{
	}

	void RigidBodyProxyComponent::registerObject()
	{
		g_typeManager->registerObject<RigidBodyProxyComponent>();
	}

	void RigidBodyProxyComponent::onEntitySet(Entity* entity)
	{
		Component::onEntitySet(entity);
	}

	void RigidBodyProxyComponent::onWorldSet(World* world)
	{
		RigidBodyComponent::onWorldSet(world);
	}

	void RigidBodyProxyComponent::onEntityRemove()
	{
		Component::onEntityRemove();
	}

	void RigidBodyProxyComponent::createPlayerController()
	{
		btTransform startTransform; startTransform.setIdentity();
		startTransform.setOrigin(glmVectorToBt(getEntity()->getPosition()));

		float fRadius = 0.4f;
		float fHeight = 0.1f;
		m_capsuleShape = mem_new<btCapsuleShape>(fRadius, fHeight);

		m_ghostObject = mem_new<btPairCachingGhostObject>();
		m_ghostObject->setWorldTransform(startTransform);

		m_ghostCallback = mem_new<btGhostPairCallback>();
		getPhysicsWorld()->getWorld()->getPairCache()->setInternalGhostPairCallback(m_ghostCallback);

		m_ghostObject->setCollisionShape(m_capsuleShape);

		m_characterController = mem_new<btKinematicCharacterController>(
			m_ghostObject, 
			m_capsuleShape, 
			0.25f, 
			btVector3(0.0f, 1.0f, 0.0f));
		
		getPhysicsWorld()->getWorld()->addCollisionObject(
			m_ghostObject//,
			//getCollisionParameter(eFilterGroup),
			//getCollisionParameter(eFilterMask)
		);

		getPhysicsWorld()->getWorld()->addAction(m_characterController);
	}

	void RigidBodyProxyComponent::update(float dt)
	{
		m_characterController->updateAction(getPhysicsWorld()->getWorld(), dt);
	}

	void RigidBodyProxyComponent::setDirection(const glm::vec3& direction)
	{
		m_characterController->setWalkDirection(glmVectorToBt(direction));
	}

	void RigidBodyProxyComponent::setPositionForce(const glm::vec3& position)
	{
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(glmVectorToBt(position));
		m_ghostObject->setWorldTransform(trans);
	}
}