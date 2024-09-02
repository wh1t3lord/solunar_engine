#include "engine.h"
#include "engine/entity/entity.h"
#include "engine/entity/component.h"
#include "engine/entity/world.h"

namespace solunar
{

IMPLEMENT_OBJECT(Entity, SerializableObject);

BEGIN_PROPERTY_REGISTER(Entity)
{
	REGISTER_PROPERTY(Entity, PropertyMatrix4, m_worldTransform);
	REGISTER_PROPERTY(Entity, PropertyBoundingBox, m_boundingBox);
	REGISTER_PROPERTY(Entity, PropertyBoundingBox, m_worldBoundingBox);
	REGISTER_PROPERTY(Entity, PropertyVector3, m_position);
	REGISTER_PROPERTY(Entity, PropertyQuaternion, m_rotation);
	REGISTER_PROPERTY(Entity, PropertyVector3, m_scale);
}
END_PROPERTY_REGISTER(Entity)

Entity::Entity() :
	m_world(nullptr),
	m_rootEntity(nullptr),
	m_worldTransform(1.0f),
	m_position(0.0f, 0.0f, 0.0f),
	m_rotation( glm::identity<glm::quat>() ),
	m_scale(1.0f, 1.0f, 1.0f)
{
	m_boundingBox.SetIdentity();
	m_worldBoundingBox.SetIdentity();
}

Entity::~Entity()
{
	for (auto& it : m_components)
	{
		it->OnEntityRemove();
		mem_delete(it);
		it = nullptr;
	}

	m_components.clear();

	m_children.clear();

	m_world = nullptr;

	m_rootEntity = nullptr;
}

void Entity::LoadXML(tinyxml2::XMLElement& element)
{
	if (strcmp(element.Name(), "Entity") != 0)
		Core::Error("Entity::LoadXML: '%s' is not a entity element.", element.Name());

	const tinyxml2::XMLAttribute* entityName = element.FindAttribute("name");
	if (entityName)
		m_name = entityName->Value();

	for (tinyxml2::XMLElement* entity = element.FirstChildElement(); entity; entity = entity->NextSiblingElement()) {
		if (strcmp(entity->Name(), "Position") == 0) {
			m_position = GetVector3FromXMLElement(*entity);
		}
		else if (strcmp(entity->Name(), "Rotation") == 0) {
			glm::vec3 rotation = GetVector3FromXMLElement(*entity);
			SetEulerRotation( glm::radians( rotation));
		}
		else if (strcmp(entity->Name(), "Scale") == 0) {
			m_scale = GetVector3FromXMLElement(*entity);
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
			const TypeInfo* typeInfo = g_typeManager->GetTypeInfoByName(entity->Name());
			if (typeInfo) {
				Component* component = CreateComponentByTypeInfo(typeInfo);
				component->LoadXML(*entity);
			}
			else {
				Core::Error("Entity::LoadXML: unknowed component! name=%s", entity->Name());
			}
		}
	}

	// Load childs
	tinyxml2::XMLElement* childElement = element.FirstChildElement("Entity");
	while (childElement)
	{
		const TypeInfo* entityTypeInfo = Entity::GetStaticTypeInfo();

		// if classid
		if (const tinyxml2::XMLAttribute* nodeClassId = childElement->FindAttribute("classId"))
		{
			entityTypeInfo = g_typeManager->GetTypeInfoById(nodeClassId->IntValue());
		}
		// if classname
		else if (const tinyxml2::XMLAttribute* nodeClassName = childElement->FindAttribute("className"))
		{
			entityTypeInfo = g_typeManager->GetTypeInfoByName(nodeClassId->Value());
		}

		Entity* entity = CreateChildEx(entityTypeInfo);
		entity->LoadXML(*childElement);

		// m_world->postInitializeNode(node);

		childElement = childElement->NextSiblingElement("Entity");
	}
}

void Entity::SaveXML(tinyxml2::XMLElement& element)
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
		if (it->IsSerializable())
		{
			const TypeInfo* typeInfo = it->GetTypeInfo();
			tinyxml2::XMLElement* compElem = element.InsertNewChildElement(typeInfo->m_name);
			it->SaveXML(*compElem);
		}
	}
}

glm::vec3 Entity::GetWorldPosition() const
{
	if (m_rootEntity) {
		m_rootEntity->UpdateWorldTransform();
		return glm::vec3(m_rootEntity->GetWorldTranslation() * glm::vec4(m_position, 1.0f));
	}

	return m_position;
}

void Entity::SetEulerRotation(const glm::vec3& rot)
{
	m_rotation = glm::quat_cast(glm::yawPitchRoll(rot.x, rot.y, rot.z));
}

glm::mat4 Entity::GetWorldTranslation()
{
//	UpdateWorldTransform();
	return m_worldTransform;
}

glm::mat4 Entity::GetLocalTranslation()
{
	glm::mat4 tranlation = glm::mat4(1.0f);
	tranlation = tranlation * glm::mat4_cast(m_rotation) * glm::scale(tranlation, m_scale);
	return tranlation;
}

void Entity::SetBoundingBox(const BoundingBox& boundingBox)
{
	m_boundingBox = boundingBox;
}

const BoundingBox& Entity::GetLocalBoundingBox()
{
	return m_boundingBox;
}

const BoundingBox& Entity::GetBoundingBox()
{
	return m_worldBoundingBox;
}

void Entity::SetRootEntity(Entity* Entity)
{
	m_rootEntity = Entity;
}

void Entity::OnWorldSet(World* world)
{
	m_world = world;
}

Entity* Entity::CreateChild()
{
	Entity* entity = m_world->CreateEntity();
	entity->SetRootEntity(this);
	m_children.push_back(entity);
	return entity;
}

Entity* Entity::CreateChildEx(const TypeInfo* typeInfo)
{
	Entity* entity = m_world->CreateEntityEx(typeInfo);
	entity->SetRootEntity(this);
	m_children.push_back(entity);
	return entity;
}

Component* Entity::CreateComponentByTypeInfo(const TypeInfo* typeinfo)
{
	Component* component = (Component*)g_typeManager->CreateObjectByTypeInfo(typeinfo);
	m_components.push_back(component);
	if (m_world) component->OnWorldSet(m_world);
	component->OnEntitySet(this);

	return component;
}

Component* Entity::GetComponentByTypeInfo(const TypeInfo* typeinfo)
{
	for (ComponentIt it = m_components.begin(); it != m_components.end(); ++it)
	{
		Component* component = *it;
		if (component)
		{
			const TypeInfo* compTypeInfo = component->GetTypeInfo();
			if (compTypeInfo && compTypeInfo->IsA(typeinfo))
				return component;
		}
	}

	return nullptr;
}

void Entity::AddComponent(Component* component)
{
	Assert(component);
	m_components.push_back(component);
	if (m_world) component->OnWorldSet(m_world);
	component->OnEntitySet(this);
}

void Entity::UpdateWorldTransform()
{
	m_worldTransform = glm::identity<glm::mat4>();

	// rotate matrix by euler
	glm::mat4 rotation = glm::mat4_cast(m_rotation);

	if (m_rootEntity)
		m_worldTransform = m_rootEntity->GetWorldTranslation() * glm::scale(m_worldTransform, m_scale) * rotation * glm::translate(m_worldTransform, m_position);
	else
		m_worldTransform = glm::scale(m_worldTransform, m_scale) * rotation * glm::translate(m_worldTransform, m_position);
}

BoundingBox Entity_TransformBox(const BoundingBox& box, const glm::mat4& m)
{
	// transform to center/extents box representation
	glm::vec3 center = (box.m_max + box.m_min) * glm::vec3(0.5);
	glm::vec3 extents = box.m_max - center;

	// transform center
	glm::vec3 t_center = glm::vec3(m * glm::vec4(center, 1.0));

	// transform extents (take maximum)
	glm::mat3 abs_mat = glm::mat3(abs(glm::vec3(m[0])), abs(glm::vec3(m[1])), abs(glm::vec3(m[2])));
	glm::vec3 t_extents = abs_mat * extents;

	// transform to min/max box representation
	glm::vec3 tmin = t_center - t_extents;
	glm::vec3 tmax = t_center + t_extents;

	BoundingBox rbox;
	rbox.SetIdentity();
	rbox.m_min = tmin;
	rbox.m_max = tmax;
	return rbox;
}

void Entity::TransformBBox()
{
	m_worldBoundingBox = Entity_TransformBox(m_boundingBox, GetWorldTranslation());
}

void Entity::QuaternionRotate(const glm::vec3& axis, float angle)
{
	float angleRad = glm::radians(angle);
	auto axisNorm = glm::normalize(axis);

	float w = glm::cos(angleRad / 2);
	float v = glm::sin(angleRad / 2);
	glm::vec3 qv = axisNorm * v;

	m_rotation = glm::quat(w, qv);
}

}