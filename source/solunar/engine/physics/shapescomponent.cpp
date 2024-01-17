#include "enginepch.h"
#include "engine/entity/entity.h"
#include "engine/entity/world.h"
#include "engine/physics/shapescomponent.h"
#include "engine/physics/rigidbodycomponent.h"
#include "engine/physics/bullet_private.h"
#include "engine/physics/physicsworld.h"

namespace engine {

	void ShapeComponent::registerObject()
	{
		g_typeManager->registerObject<ShapeComponent>();
	}

	ShapeComponent::ShapeComponent()
	{
		m_localPosition = glm::vec3(0.0f);
	}

	ShapeComponent::~ShapeComponent()
	{

	}

	void ShapeComponent::onEntitySet(Entity* entity)
	{
		Component::onEntitySet(entity);
	}

	void ShapeComponent::onEntityRemove()
	{
		if (getEntity())
		{
			if (m_rigidBody)
			{
				Assert2(m_rigidBody->getCompoundShape(), "RigidBody is not initialized!");
				m_rigidBody->getCompoundShape()->removeChildShape(m_shape);
			}
		}

		// destroy shape
		mem_delete(m_shape);
	}

	void ShapeComponent::onWorldSet(World* world)
	{
		Component::onWorldSet(world);
		//m_physicsWorld = world->getPhysicsWorld();
	}

	void ShapeComponent::loadXML(tinyxml2::XMLElement& element)
	{
		tinyxml2::XMLElement* positionElement = element.FirstChildElement("Position");
		if (positionElement)
		{
			m_localPosition = getVector3FromXMLElement(*positionElement);
		}
	}

	void ShapeComponent::saveXML(tinyxml2::XMLElement& element)
	{
	}

	void ShapeComponent::initializeShape()
	{
		m_rigidBody = getEntity()->getComponent<RigidBodyComponent>();
		if (!m_rigidBody)
		{
			m_rigidBody = getEntity()->createComponent<RigidBodyComponent>();
		}

		createShapeInternal();

		Assert2(m_shape, "Failed to allocate or create shape.");

		initializeShapeTransform();

		// apply created shape to rigid body compound shape
		Assert2(m_rigidBody->getCompoundShape(), "RigidBody is not initialized!");
		m_rigidBody->getCompoundShape()->addChildShape(m_btLocalTransform, m_shape);
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

	void BoxShapeComponent::registerObject()
	{
		g_typeManager->registerObject<BoxShapeComponent>();
	}

	BoxShapeComponent::BoxShapeComponent()
	{
		m_size = glm::vec3(1.0f);
	}

	BoxShapeComponent::~BoxShapeComponent()
	{
	}

	void BoxShapeComponent::loadXML(tinyxml2::XMLElement& element)
	{
		ShapeComponent::loadXML(element);

		tinyxml2::XMLElement* sizeElement = element.FirstChildElement("Size");
		if (sizeElement)
		{
			m_size = getVector3FromXMLElement(*sizeElement);
		}
	}

	void BoxShapeComponent::saveXML(tinyxml2::XMLElement& element)
	{
		ShapeComponent::saveXML(element);

		tinyxml2::XMLElement* sizeElement = element.InsertNewChildElement("Size");
		if (sizeElement)
		{
			saveVector3ToXMLElement(*sizeElement, m_size);
		}
	}

	void BoxShapeComponent::createShapeInternal()
	{
		m_shape = (btCollisionShape*)mem_new<btBoxShape>(glmVectorToBt(m_size));
	}

	//////////////////////////////////////////////////////////////////////////
	// Sphere Shape Component

	void SphereShapeComponent::registerObject()
	{
		g_typeManager->registerObject<SphereShapeComponent>();
	}

	SphereShapeComponent::SphereShapeComponent()
	{
		m_fRadius = 1.0f;
	}

	SphereShapeComponent::~SphereShapeComponent()
	{

	}

	void SphereShapeComponent::loadXML(tinyxml2::XMLElement& element)
	{
		ShapeComponent::loadXML(element);

		tinyxml2::XMLElement* sizeElement = element.FirstChildElement("Size");
		if (sizeElement)
		{
			const tinyxml2::XMLAttribute* attrR = sizeElement->FindAttribute("r");
			if(attrR) m_fRadius = attrR->FloatValue();
		}
	}

	void SphereShapeComponent::saveXML(tinyxml2::XMLElement& element)
	{
		ShapeComponent::saveXML(element);

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

	void CylinderShapeComponent::registerObject()
	{
		g_typeManager->registerObject<CylinderShapeComponent>();
	}

	CylinderShapeComponent::CylinderShapeComponent()
	{
		m_size = glm::vec3(1.0f);
	}

	CylinderShapeComponent::~CylinderShapeComponent()
	{
	}

	void CylinderShapeComponent::loadXML(tinyxml2::XMLElement& element)
	{
		ShapeComponent::loadXML(element);

		tinyxml2::XMLElement* sizeElement = element.FirstChildElement("Size");
		if (sizeElement)
		{
			m_size = getVector3FromXMLElement(*sizeElement);
		}
	}

	void CylinderShapeComponent::saveXML(tinyxml2::XMLElement& element)
	{
		ShapeComponent::saveXML(element);

		tinyxml2::XMLElement* sizeElement = element.InsertNewChildElement("Size");
		if (sizeElement)
		{
			saveVector3ToXMLElement(*sizeElement, m_size);
		}
	}

	void CylinderShapeComponent::createShapeInternal()
	{
		m_shape = (btCollisionShape*)mem_new<btCylinderShape>(glmVectorToBt(m_size));
	}


	//////////////////////////////////////////////////////////////////////////
	// Capsule Shape Component

	void CapsuleShapeComponent::registerObject()
	{
		g_typeManager->registerObject<CapsuleShapeComponent>();
	}

	CapsuleShapeComponent::CapsuleShapeComponent()
	{
		m_fRadius = 1.0f;
		m_fHeight = 1.0f;
	}

	CapsuleShapeComponent::~CapsuleShapeComponent()
	{

	}

	void CapsuleShapeComponent::loadXML(tinyxml2::XMLElement& element)
	{
		ShapeComponent::loadXML(element);

		tinyxml2::XMLElement* sizeElement = element.FirstChildElement("Size");
		if (sizeElement)
		{
			const tinyxml2::XMLAttribute* attrR = sizeElement->FindAttribute("r");
			if (attrR) m_fRadius = attrR->FloatValue();

			const tinyxml2::XMLAttribute* attrH = sizeElement->FindAttribute("h");
			if (attrH) m_fHeight = attrH->FloatValue();
		}
	}

	void CapsuleShapeComponent::saveXML(tinyxml2::XMLElement& element)
	{
		ShapeComponent::saveXML(element);

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
