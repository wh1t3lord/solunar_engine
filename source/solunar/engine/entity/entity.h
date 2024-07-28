#ifndef ENGINE_ENTITY_ENTITY_H
#define ENGINE_ENTITY_ENTITY_H

#include "core/math/boundingbox.h"

namespace engine
{

class World;
class Component;

class Entity : public SerializableObject
{
	friend class World;
public:
	ImplementObject(Entity, SerializableObject);
	DeclarePropertyRegister(Entity);

public:
	Entity();
	~Entity();

	virtual void loadXML(tinyxml2::XMLElement& element);
	virtual void saveXML(tinyxml2::XMLElement& element);

	virtual void onWorldSet(World* world);

	void setRootEntity(Entity* node);

	glm::vec3 getPosition()	const	{ return m_position; }
	glm::quat getRotation()	const	{ return m_rotation; }
	glm::vec3 getScale()	const	{ return m_scale; }

	void setPosition(const glm::vec3& pos) { m_position = pos; }
	void setRotation(const glm::quat& rot) { m_rotation = rot; }
	void setEulerRotation(const glm::vec3& rot);
	void setScale(const glm::vec3& scale) { m_scale = scale; }

	void quaternionRotate(const glm::vec3& axis, float angle);

	glm::mat4 getWorldTranslation();
	glm::mat4 getLocalTranslation();

	void setBoundingBox(const BoundingBox& boundingBox);
	const BoundingBox& getLocalBoundingBox();
	const BoundingBox& getBoundingBox();

	Entity* createChild();
	Entity* createChildEx(const TypeInfo* typeInfo);

	Component* createComponentByTypeInfo(const TypeInfo* typeinfo);

	template <typename T>
	T* createComponent();

	Component* getComponentByTypeInfo(const TypeInfo* typeinfo);

	template <typename T>
	T* getComponent();

	template <typename T>
	inline std::vector<T*> getComponents();

	void addComponent(Component* component);

	void updateWorldTransform();

private:
	void transformBBox();

private:
	typedef std::vector<Component*>::iterator ComponentIt;
	std::vector<Component*> m_components;

	typedef std::vector<Entity*>::iterator EntityIt;
	std::vector<Entity*> m_children;

	World* m_world;

	Entity* m_rootEntity;

	std::string m_name;

	glm::mat4 m_worldTransform;
	BoundingBox m_boundingBox;
	BoundingBox m_worldBoundingBox;
	glm::quat m_rotation;
	glm::vec3 m_position;
	glm::vec3 m_scale;
};

}

#include "engine/entity/entity.inl"

#endif