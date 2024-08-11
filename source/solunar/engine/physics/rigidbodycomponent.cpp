#include "enginepch.h"
#include "engine/physics/rigidbodycomponent.h"
#include "engine/physics/physicsworld.h"
#include "engine/physics/shapescomponent.h"

#include "engine/entity/world.h"

#include "core/timer.h"

namespace solunar {

	BEGIN_PROPERTY_REGISTER(RigidBodyComponent)
	{
		REGISTER_PROPERTY(RigidBodyComponent, PropertyFloat, m_mass);
		REGISTER_PROPERTY(RigidBodyComponent, PropertyBool, m_isEnable);
		REGISTER_PROPERTY(RigidBodyComponent, PropertyBool, m_static);
		REGISTER_PROPERTY(RigidBodyComponent, PropertyBool, m_isKinematic);
	//	RegisterProperty(RigidBodyComponent, PropertyBool, m_inWorld); INTERNAL !!!
	}
	END_PROPERTY_REGISTER(RigidBodyComponent)

	RigidBodyComponent::RigidBodyComponent()
	{
		m_rigidBody = nullptr;
		m_compoundShape = nullptr;
		m_physicsWorld = nullptr;

		m_mass = 0.0f;
		m_isEnable = false;
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

	void RigidBodyComponent::RegisterObject()
	{
		g_typeManager->RegisterObject<RigidBodyComponent>();
	}

	void RigidBodyComponent::OnEntitySet(Entity* entity)
	{
		Component::OnEntitySet(entity);

		if (m_physicsWorld && !m_rigidBody)
			CreateBody();
	}

	void RigidBodyComponent::OnWorldSet(World* world)
	{
		Component::OnWorldSet(world);
		m_physicsWorld = world->GetPhysicsWorld();
	}

	void RigidBodyComponent::OnEntityRemove()
	{
		DestroyBody();
		Component::OnEntityRemove();
	}

	void RigidBodyComponent::LoadXML(tinyxml2::XMLElement& element)
	{
		tinyxml2::XMLElement* staticElement = element.FirstChildElement("Static");
		if (staticElement)
		{
			staticElement->QueryBoolAttribute("value", &m_static);
		}

		CreateBody();
		UpdateBodyTranslationDirty();
	}

	void RigidBodyComponent::SaveXML(tinyxml2::XMLElement& element)
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

	void RigidBodyComponent::UpdateEntityTranslationDirty()
	{
		btTransform trans = m_rigidBody->getWorldTransform();
		GetEntity()->SetPosition(btVectorToGlm(trans.getOrigin()));

		// calculate rotation based on euler angles
		float roll, pitch, yaw;
		trans.getRotation().getEulerZYX(yaw, pitch, roll);
		GetEntity()->SetEulerRotation(glm::vec3(roll, pitch, yaw));
	}

	void RigidBodyComponent::UpdateBodyTranslationDirty()
	{
		if (!m_rigidBody)
			return;

		btTransform trans;
		trans.setIdentity();

		// origin
		trans.setOrigin(glmVectorToBt(GetEntity()->GetPosition()));

		// rotation
		glm::quat entityRot = GetEntity()->GetRotation();
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

		UpdateBodyTranslationDirty();
	}
#endif

	void RigidBodyComponent::CreateBody()
	{
		Assert(m_physicsWorld);

		if (m_rigidBody)
			return;

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

	void RigidBodyComponent::DestroyBody()
	{
		if (m_physicsWorld)
		{
			btDynamicsWorld* physicsWorld = m_physicsWorld->GetWorld();
			physicsWorld->removeRigidBody(m_rigidBody);
		}
		else
		{
			Core::Msg("RigidBodyComponent::DestroyBody: Can't remove body. Physics world is already destroyed.");
			Core::Msg("Entity=0x%p", GetEntity());
		}
		
		// remove user pointer from this component
		m_rigidBody->setUserPointer(nullptr);

		mem_delete(m_rigidBody);
		m_rigidBody = nullptr;

		mem_delete(m_compoundShape);
		m_compoundShape = nullptr;
	}

	void RigidBodyComponent::UpdateBodyProperties()
	{
	}

	void RigidBodyComponent::DisableCollide()
	{
		m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}

	void RigidBodyComponent::EnableCollide()
	{
		m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}

	void RigidBodyComponent::AttachShape(ShapeComponent* shape)
	{
		shape->initializeShape();
	}

	void RigidBodyComponent::DettachShape(ShapeComponent* shape)
	{
		m_compoundShape->removeChildShape(shape->getSdkShape());
	}

	void RigidBodyComponent::ApplyImpulse(const glm::vec3& impulse)
	{
		m_rigidBody->applyCentralImpulse(glmVectorToBt(impulse));
	}

	void RigidBodyComponent::SetLinearVelocity(const glm::vec3& velocity)
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
		GetPhysicsWorld()->GetWorld()->removeAction(m_characterController);
		GetPhysicsWorld()->GetWorld()->removeCollisionObject(m_ghostObject);
		m_ghostObject->setCollisionShape(nullptr);
		GetPhysicsWorld()->GetWorld()->getPairCache()->setInternalGhostPairCallback(nullptr);

		mem_delete(m_capsuleShape);
		mem_delete(m_ghostCallback);
		mem_delete(m_ghostObject);
		mem_delete(m_characterController);
	}

	void RigidBodyProxyComponent::RegisterObject()
	{
		g_typeManager->RegisterObject<RigidBodyProxyComponent>();
	}

	void RigidBodyProxyComponent::OnEntitySet(Entity* entity)
	{
		Component::OnEntitySet(entity);
	}

	void RigidBodyProxyComponent::OnWorldSet(World* world)
	{
		RigidBodyComponent::OnWorldSet(world);
	}

	void RigidBodyProxyComponent::OnEntityRemove()
	{
		Component::OnEntityRemove();
	}

	void RigidBodyProxyComponent::CreatePlayerController()
	{
		btTransform startTransform; startTransform.setIdentity();
		startTransform.setOrigin(glmVectorToBt(GetEntity()->GetPosition()));

		float fRadius = 0.4f;
		float fHeight = 0.1f;
		m_capsuleShape = mem_new<btCapsuleShape>(fRadius, fHeight);

		m_ghostObject = mem_new<btPairCachingGhostObject>();
		m_ghostObject->setWorldTransform(startTransform);
		m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

		m_ghostCallback = mem_new<btGhostPairCallback>();
		GetPhysicsWorld()->GetWorld()->getPairCache()->setInternalGhostPairCallback(m_ghostCallback);

		m_ghostObject->setCollisionShape(m_capsuleShape);

		m_characterController = mem_new<btKinematicCharacterController>(
			m_ghostObject, 
			m_capsuleShape, 
			0.25f, 
			btVector3(0.0f, 1.0f, 0.0f));

		m_characterController->setMaxJumpHeight(0.8f);
		m_characterController->setGravity(GetPhysicsWorld()->GetWorld()->getGravity());

		GetPhysicsWorld()->GetWorld()->addCollisionObject(
			m_ghostObject,
			btBroadphaseProxy::CharacterFilter,
			btBroadphaseProxy::AllFilter
		);

		GetPhysicsWorld()->GetWorld()->addAction(m_characterController);
	}

	void RigidBodyProxyComponent::Update(float dt)
	{
		m_characterController->updateAction(GetPhysicsWorld()->GetWorld(), dt);
	}

	void RigidBodyProxyComponent::SetDirection(const glm::vec3& direction)
	{
		m_characterController->setWalkDirection(glmVectorToBt(direction));
	}

	void RigidBodyProxyComponent::SetPositionForce(const glm::vec3& position)
	{
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(glmVectorToBt(position));
		m_ghostObject->setWorldTransform(trans);
	}
}