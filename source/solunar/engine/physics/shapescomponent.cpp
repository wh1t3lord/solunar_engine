#include "enginepch.h"
#include "engine/entity/entity.h"
#include "engine/entity/world.h"
#include "engine/physics/shapescomponent.h"
#include "engine/physics/rigidbodycomponent.h"
#include "engine/physics/bullet_private.h"
#include "engine/physics/physicsworld.h"

namespace solunar {

	void ShapeComponent::RegisterObject()
	{
		g_typeManager->RegisterObject<ShapeComponent>();
	}

	ShapeComponent::ShapeComponent() :
		m_shape(nullptr),
		m_physicsWorld(nullptr),
		m_rigidBody(nullptr),
		m_scale(glm::vec3(1.0f)),
		m_rotation( glm::identity<glm::quat>())
	{
		m_localPosition = glm::vec3(0.0f);
	}

	ShapeComponent::~ShapeComponent()
	{

	}

	void ShapeComponent::OnEntitySet(Entity* entity)
	{
		Component::OnEntitySet(entity);
	}

	void ShapeComponent::OnEntityRemove()
	{
		if (GetEntity())
		{
			// HACK: I don't know how it's brokes but... hack...
#if 1
			if (RigidBodyComponent* rigidBody = GetEntity()->GetComponent<RigidBodyComponent>())
			{
				Assert2(rigidBody->GetCompoundShape(), "RigidBody is not initialized!");
				rigidBody->GetCompoundShape()->removeChildShape(m_shape);
			}
#else
			if (m_rigidBody)
			{
				Assert2(m_rigidBody->GetCompoundShape(), "RigidBody is not initialized!");
				m_rigidBody->GetCompoundShape()->removeChildShape(m_shape);
			}
#endif
		}

		// destroy shape
		mem_delete(m_shape);
	}

	void ShapeComponent::OnWorldSet(World* world)
	{
		Component::OnWorldSet(world);
		m_physicsWorld = world->GetPhysicsWorld();
	}

	void ShapeComponent::LoadXML(tinyxml2::XMLElement& element)
	{
		tinyxml2::XMLElement* positionElement = element.FirstChildElement("Position");
		if (positionElement)
		{
			m_localPosition = GetVector3FromXMLElement(*positionElement);
		}
	}

	void ShapeComponent::SaveXML(tinyxml2::XMLElement& element)
	{
		tinyxml2::XMLElement* positionElement = element.InsertNewChildElement("Position");
		SaveVector3ToXMLElement(*positionElement, m_localPosition);
	}

	void ShapeComponent::initializeShape()
	{
		m_rigidBody = GetEntity()->GetComponent<RigidBodyComponent>();
		if (!m_rigidBody)
		{
			m_rigidBody = GetEntity()->CreateComponent<RigidBodyComponent>();
		}

		createShapeInternal();

		Assert2(m_shape, "Failed to allocate or create shape.");

		initializeShapeTransform();

		// apply created shape to rigid body compound shape
		Assert2(m_rigidBody->GetCompoundShape(), "RigidBody is not initialized!");
		m_rigidBody->GetCompoundShape()->addChildShape(m_btLocalTransform, m_shape);
	}

	void ShapeComponent::initializeShapeTransform()
	{
		// identity
		m_btLocalTransform.setIdentity();

		// apply transform
		m_btLocalTransform.setOrigin(glmVectorToBt(m_localPosition));
	}

	void ShapeComponent::createShapeInternal()
	{
		Assert2(0, "Pure virtual call");
	}

	//////////////////////////////////////////////////////////////////////////
	// Box Shape Component

	void BoxShapeComponent::RegisterObject()
	{
		g_typeManager->RegisterObject<BoxShapeComponent>();
	}

	BoxShapeComponent::BoxShapeComponent()
	{
		m_size = glm::vec3(1.0f);
	}

	BoxShapeComponent::~BoxShapeComponent()
	{
	}

	void BoxShapeComponent::LoadXML(tinyxml2::XMLElement& element)
	{
		ShapeComponent::LoadXML(element);

		tinyxml2::XMLElement* sizeElement = element.FirstChildElement("Size");
		if (sizeElement)
		{
			m_size = GetVector3FromXMLElement(*sizeElement);
		}
	}

	void BoxShapeComponent::SaveXML(tinyxml2::XMLElement& element)
	{
		ShapeComponent::SaveXML(element);

		tinyxml2::XMLElement* sizeElement = element.InsertNewChildElement("Size");
		if (sizeElement)
		{
			SaveVector3ToXMLElement(*sizeElement, m_size);
		}
	}

	void BoxShapeComponent::createShape(const glm::vec3& size)
	{
		m_size = size;
		createShapeInternal();
	}

	void BoxShapeComponent::createShapeInternal()
	{
		m_shape = (btCollisionShape*)mem_new<btBoxShape>(glmVectorToBt(m_size));
	}

	//////////////////////////////////////////////////////////////////////////
	// Sphere Shape Component

	void SphereShapeComponent::RegisterObject()
	{
		g_typeManager->RegisterObject<SphereShapeComponent>();
	}

	SphereShapeComponent::SphereShapeComponent()
	{
		m_fRadius = 1.0f;
	}

	SphereShapeComponent::~SphereShapeComponent()
	{

	}

	void SphereShapeComponent::LoadXML(tinyxml2::XMLElement& element)
	{
		ShapeComponent::LoadXML(element);

		tinyxml2::XMLElement* sizeElement = element.FirstChildElement("Size");
		if (sizeElement)
		{
			const tinyxml2::XMLAttribute* attrR = sizeElement->FindAttribute("r");
			if(attrR) m_fRadius = attrR->FloatValue();
		}
	}

	void SphereShapeComponent::SaveXML(tinyxml2::XMLElement& element)
	{
		ShapeComponent::SaveXML(element);

		tinyxml2::XMLElement* sizeElement = element.InsertNewChildElement("Size");
		if (sizeElement)
		{
			sizeElement->SetAttribute("r", m_fRadius);
		}
	}

	void SphereShapeComponent::createShapeInternal()
	{
		m_shape = (btCollisionShape*)mem_new<btSphereShape>(m_fRadius);
	}


	//////////////////////////////////////////////////////////////////////////
	// Cylinder Shape Component

	void CylinderShapeComponent::RegisterObject()
	{
		g_typeManager->RegisterObject<CylinderShapeComponent>();
	}

	CylinderShapeComponent::CylinderShapeComponent()
	{
		m_size = glm::vec3(1.0f);
	}

	CylinderShapeComponent::~CylinderShapeComponent()
	{
	}

	void CylinderShapeComponent::LoadXML(tinyxml2::XMLElement& element)
	{
		ShapeComponent::LoadXML(element);

		tinyxml2::XMLElement* sizeElement = element.FirstChildElement("Size");
		if (sizeElement)
		{
			m_size = GetVector3FromXMLElement(*sizeElement);
		}
	}

	void CylinderShapeComponent::SaveXML(tinyxml2::XMLElement& element)
	{
		ShapeComponent::SaveXML(element);

		tinyxml2::XMLElement* sizeElement = element.InsertNewChildElement("Size");
		if (sizeElement)
		{
			SaveVector3ToXMLElement(*sizeElement, m_size);
		}
	}

	void CylinderShapeComponent::createShapeInternal()
	{
		m_shape = (btCollisionShape*)mem_new<btCylinderShape>(glmVectorToBt(m_size));
	}


	//////////////////////////////////////////////////////////////////////////
	// Capsule Shape Component

	void CapsuleShapeComponent::RegisterObject()
	{
		g_typeManager->RegisterObject<CapsuleShapeComponent>();
	}

	CapsuleShapeComponent::CapsuleShapeComponent()
	{
		m_fRadius = 1.0f;
		m_fHeight = 1.0f;
	}

	CapsuleShapeComponent::~CapsuleShapeComponent()
	{

	}

	void CapsuleShapeComponent::createCapsule(float _fRadius, float _fHeight)
	{
		m_fRadius = _fRadius;
		m_fHeight = _fHeight;
		createShapeInternal();
	}

	void CapsuleShapeComponent::LoadXML(tinyxml2::XMLElement& element)
	{
		ShapeComponent::LoadXML(element);

		tinyxml2::XMLElement* sizeElement = element.FirstChildElement("Size");
		if (sizeElement)
		{
			const tinyxml2::XMLAttribute* attrR = sizeElement->FindAttribute("r");
			if (attrR) m_fRadius = attrR->FloatValue();

			const tinyxml2::XMLAttribute* attrH = sizeElement->FindAttribute("h");
			if (attrH) m_fHeight = attrH->FloatValue();
		}
	}

	void CapsuleShapeComponent::SaveXML(tinyxml2::XMLElement& element)
	{
		ShapeComponent::SaveXML(element);

		tinyxml2::XMLElement* sizeElement = element.InsertNewChildElement("Size");
		if (sizeElement)
		{
			sizeElement->SetAttribute("r", m_fRadius);
			sizeElement->SetAttribute("h", m_fHeight);
		}
	}

	void CapsuleShapeComponent::createShapeInternal()
	{
		m_shape = (btCollisionShape*)mem_new<btCapsuleShape>(m_fRadius, m_fHeight);
	}
}
