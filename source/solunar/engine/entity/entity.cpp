#include "engine.h"
#include "engine/entity/entity.h"
#include "engine/entity/component.h"
#include "engine/entity/world.h"

namespace solunar
{

BeginPropertyRegister(Entity)
{
	RegisterProperty(Entity, PropertyMatrix4, m_worldTransform);
	RegisterProperty(Entity, PropertyBoundingBox, m_boundingBox);
	RegisterProperty(Entity, PropertyBoundingBox, m_worldBoundingBox);
	RegisterProperty(Entity, PropertyVector3, m_position);
	RegisterProperty(Entity, PropertyQuaternion, m_rotation);
	RegisterProperty(Entity, PropertyVector3, m_scale);
}
EndPropertyRegister(Entity)

Entity::Entity() :
	m_world(nullptr),
	m_rootEntity(nullptr),
	m_worldTransform(1.0f),
	m_position(0.0f, 0.0f, 0.0f),
	m_rotation( glm::identity<glm::quat>() ),
	m_scale(1.0f, 1.0f, 1.0f)
{
	m_boundingBox.setIdentity();
	m_worldBoundingBox.setIdentity();
}

Entity::~Entity()
{
	for (auto& it : m_components)
	{
		it->onEntityRemove();
		mem_delete(it);
		it = nullptr;
	}

	m_components.clear();

	m_children.clear();

	m_world = nullptr;

	m_rootEntity = nullptr;
}

void Entity::loadXML(tinyxml2::XMLElement& element)
{
	if (strcmp(element.Name(), "Entity") != 0)
		Core::error("Entity::loadXML: '%s' is not a entity element.", element.Name());

	const tinyxml2::XMLAttribute* entityName = element.FindAttribute("name");
	if (entityName)
		m_name = entityName->Value();

	for (tinyxml2::XMLElement* entity = element.FirstChildElement(); entity; entity = entity->NextSiblingElement()) {
		if (strcmp(entity->Name(), "Position") == 0) {
			m_position = getVector3FromXMLElement(*entity);
		}
		else if (strcmp(entity->Name(), "Rotation") == 0) {
			glm::vec3 rotation = getVector3FromXMLElement(*entity);
			setEulerRotation( glm::radians( rotation));
		}
		else if (strcmp(entity->Name(), "Scale") == 0) {
			m_scale = getVector3FromXMLElement(*entity);
		}
		else if (strcmp(entity->Name(), "BoundingBox") == 0) {
			tinyxml2::XMLElement* bboxMin = entity->FirstChildElement("Min");
			bboxMin->QueryFloatAttribute("x", &m_boundingBox.m_min.x);
			bboxMin->QueryFloatAttribute("y", &m_boundingBox.m_min.y);
			bboxMin->QueryFloatAttribute("z", &m_boundingBox.m_min.z);
			
			tinyxml2::XMLElement* bboxMax = entity->FirstChildElement("Max");
			bboxMax->QueryFloatAttribute("x", &m_boundingBox.m_max.x);
			bboxMax->QueryFloatAttribute("y", &m_boundingBox.m_max.y);
			bboxMax->QueryFloatAttribute("z", &m_boundingBox.m_max.z);
		}
		else if (strcmp(entity->Name(), "Entity") != 0) { // components
			const TypeInfo* typeInfo = g_typeManager->getTypeInfoByName(entity->Name());
			if (typeInfo) {
				Component* component = createComponentByTypeInfo(typeInfo);
				component->loadXML(*entity);
			}
			else {
				Core::error("Entity::loadXML: unknowed component! name=%s", entity->Name());
			}
		}
	}

	// load childs
	tinyxml2::XMLElement* childElement = element.FirstChildElement("Entity");
	while (childElement)
	{
		const TypeInfo* entityTypeInfo = Entity::getStaticTypeInfo();

		// if classid
		if (const tinyxml2::XMLAttribute* nodeClassId = childElement->FindAttribute("classId"))
		{
			entityTypeInfo = g_typeManager->getTypeInfoById(nodeClassId->IntValue());
		}
		// if classname
		else if (const tinyxml2::XMLAttribute* nodeClassName = childElement->FindAttribute("className"))
		{
			entityTypeInfo = g_typeManager->getTypeInfoByName(nodeClassId->Value());
		}

		Entity* entity = createChildEx(entityTypeInfo);
		entity->loadXML(*childElement);

		// m_world->postInitializeNode(node);

		childElement = childElement->NextSiblingElement("Entity");
	}
}

void Entity::saveXML(tinyxml2::XMLElement& element)
{
	if (!m_name.empty())
		element.SetAttribute("name", m_name.c_str());

	tinyxml2::XMLElement* position = element.InsertNewChildElement("Position");
	if (position)
	{
		position->SetAttribute("x", m_position.x);
		position->SetAttribute("y", m_position.y);
		position->SetAttribute("z", m_position.z);
	}

	tinyxml2::XMLElement* rotation = element.InsertNewChildElement("Rotation");
	if (rotation)
	{
		rotation->SetAttribute("x", m_rotation.x);
		rotation->SetAttribute("y", m_rotation.y);
		rotation->SetAttribute("z", m_rotation.z);
	}


	tinyxml2::XMLElement* scale = element.InsertNewChildElement("Scale");
	if (scale)
	{
		scale->SetAttribute("x", m_scale.x);
		scale->SetAttribute("y", m_scale.y);
		scale->SetAttribute("z", m_scale.z);
	}

	tinyxml2::XMLElement* bbox = element.InsertNewChildElement("BoundingBox");
	if (bbox)
	{
		tinyxml2::XMLElement* bboxMin = bbox->InsertNewChildElement("Min");
		bboxMin->SetAttribute("x", m_boundingBox.m_min.x);
		bboxMin->SetAttribute("y", m_boundingBox.m_min.y);
		bboxMin->SetAttribute("z", m_boundingBox.m_min.z);

		tinyxml2::XMLElement* bboxMax = bbox->InsertNewChildElement("Max");
		bboxMax->SetAttribute("x", m_boundingBox.m_max.x);
		bboxMax->SetAttribute("y", m_boundingBox.m_max.y);
		bboxMax->SetAttribute("z", m_boundingBox.m_max.z);
	}

	// save components
	for (auto it : m_components)
	{
		if (it->isSerializable())
		{
			const TypeInfo* typeInfo = it->getTypeInfo();
			tinyxml2::XMLElement* compElem = element.InsertNewChildElement(typeInfo->m_name);
			it->saveXML(*compElem);
		}
	}
}

void Entity::setEulerRotation(const glm::vec3& rot)
{
	m_rotation = glm::quat_cast(glm::yawPitchRoll(rot.x, rot.y, rot.z));
}

glm::mat4 Entity::getWorldTranslation()
{
//	updateWorldTransform();
	return m_worldTransform;
}

glm::mat4 Entity::getLocalTranslation()
{
	glm::mat4 tranlation = glm::mat4(1.0f);
	tranlation = tranlation * glm::mat4_cast(m_rotation) * glm::scale(tranlation, m_scale);
	return tranlation;
}

void Entity::setBoundingBox(const BoundingBox& boundingBox)
{
	m_boundingBox = boundingBox;
}

const BoundingBox& Entity::getLocalBoundingBox()
{
	return m_boundingBox;
}

const BoundingBox& Entity::getBoundingBox()
{
	return m_worldBoundingBox;
}

void Entity::setRootEntity(Entity* Entity)
{
	m_rootEntity = Entity;
}

glm::vec3 Entity::getWorldPosition() const
{
	if (m_rootEntity) {
		m_rootEntity->updateWorldTransform();
		return glm::vec3(m_rootEntity->getWorldTranslation() * glm::vec4(m_position, 1.0f));
	}
	
	return m_position;
}

void Entity::onWorldSet(World* world)
{
	m_world = world;
}

Entity* Entity::createChild()
{
	Entity* entity = m_world->createEntity();
	entity->setRootEntity(this);
	m_children.push_back(entity);
	return entity;
}

Entity* Entity::createChildEx(const TypeInfo* typeInfo)
{
	Entity* entity = m_world->createEntityEx(typeInfo);
	entity->setRootEntity(this);
	m_children.push_back(entity);
	return entity;
}

Component* Entity::createComponentByTypeInfo(const TypeInfo* typeinfo)
{
	Component* component = (Component*)g_typeManager->createObjectByTypeInfo(typeinfo);
	m_components.push_back(component);
	if (m_world) component->onWorldSet(m_world);
	component->onEntitySet(this);

	return component;
}

Component* Entity::getComponentByTypeInfo(const TypeInfo* typeinfo)
{
	for (ComponentIt it = m_components.begin(); it != m_components.end(); ++it)
	{
		Component* component = *it;
		if (component && component->isA(typeinfo))
		{
			return component;
		}
	}

	return nullptr;
}

void Entity::addComponent(Component* component)
{
	Assert(component);
	m_components.push_back(component);
	if (m_world) component->onWorldSet(m_world);
	component->onEntitySet(this);
}

void Entity::updateWorldTransform()
{
	m_worldTransform = glm::identity<glm::mat4>();

	// rotate matrix by euler
	glm::mat4 rotation = glm::mat4_cast(m_rotation);

	if (m_rootEntity)
		m_worldTransform = m_rootEntity->getWorldTranslation() * glm::scale(m_worldTransform, m_scale) * rotation * glm::translate(m_worldTransform, m_position);
	else
		m_worldTransform = glm::scale(m_worldTransform, m_scale) * rotation * glm::translate(m_worldTransform, m_position);
}

void Entity::transformBBox()
{
	m_worldBoundingBox = m_boundingBox;
	m_worldBoundingBox.transformAABB(getWorldTranslation());
}

void Entity::quaternionRotate(const glm::vec3& axis, float angle)
{
	float angleRad = glm::radians(angle);
	auto axisNorm = glm::normalize(axis);

	float w = glm::cos(angleRad / 2);
	float v = glm::sin(angleRad / 2);
	glm::vec3 qv = axisNorm * v;

	m_rotation = glm::quat(w, qv);
}

}