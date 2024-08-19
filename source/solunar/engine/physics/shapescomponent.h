#ifndef SHAPESCOMPONENT_H
#define SHAPESCOMPONENT_H

#include "engine/physics/bullet_private.h"
#include "engine/physics/rigidbodycomponent.h"

namespace solunar {

	//! Enumeration of shape types.
	enum class ShapeType
	{
		ShapeBox,
		ShapeSphere,
		ShapeCylinder,
		ShapeCapsule,
		ShapeTrimesh,
		ShapeConvexHull,
	};

	class RigidBodyComponent;

	//! Interface class for any type of rigid body shapes.
	class ShapeComponent : public Component
	{
		IMPLEMENT_OBJECT(ShapeComponent, Component);
	public:
		static void RegisterObject();

	public:
		ShapeComponent();
		virtual ~ShapeComponent();

		void OnEntitySet(Entity* entity) override;
		void OnEntityRemove() override;
		void OnWorldSet(World* world) override;

		// serialization
		virtual void LoadXML(tinyxml2::XMLElement& element);
		virtual void SaveXML(tinyxml2::XMLElement& element);

		//! Public initialization of shape
		void InitializeShape();

		//! Get Physics SDK shape.
		btCollisionShape* getSdkShape() { return m_shape; }

	private:
		void InitializeShapeTransform();

	protected:
		//! Base virtual function for shape creation.
		virtual void CreateShapeInternal();

	protected:
		//! Instance of current entity rigid body component.
		RigidBodyComponent* m_rigidBody;

		//! Instance of physics world in entity created in the world.
		PhysicsWorld* m_physicsWorld;

		//! SDK shape.
		btCollisionShape* m_shape;

		// Bullet physics transform
		btTransform m_btLocalTransform;

		// local shape transform
		glm::quat m_rotation;
		glm::vec3 m_localPosition;
		glm::vec3 m_scale;

	};

	class BoxShapeComponent : public ShapeComponent
	{
		IMPLEMENT_OBJECT(BoxShapeComponent, ShapeComponent);
	public:
		static void RegisterObject();

	public:
		BoxShapeComponent();
		~BoxShapeComponent();

		void LoadXML(tinyxml2::XMLElement& element) override;
		void SaveXML(tinyxml2::XMLElement& element) override;

		void createShape(const glm::vec3& size);

	private:
		void CreateShapeInternal() override;

	private:
		glm::vec3 m_size;
	};

	class SphereShapeComponent : public ShapeComponent
	{
		IMPLEMENT_OBJECT(SphereShapeComponent, ShapeComponent);
	public:
		static void RegisterObject();

	public:
		SphereShapeComponent();
		~SphereShapeComponent();

		void LoadXML(tinyxml2::XMLElement& element) override;
		void SaveXML(tinyxml2::XMLElement& element) override;

	private:
		void CreateShapeInternal() override;

	private:
		float m_fRadius;

	};

	class CylinderShapeComponent : public ShapeComponent
	{
		IMPLEMENT_OBJECT(CylinderShapeComponent, ShapeComponent);
	public:
		static void RegisterObject();

	public:
		CylinderShapeComponent();
		~CylinderShapeComponent();

		void LoadXML(tinyxml2::XMLElement& element) override;
		void SaveXML(tinyxml2::XMLElement& element) override;

	private:
		void CreateShapeInternal() override;

	private:
		glm::vec3 m_size;

	};

	class CapsuleShapeComponent : public ShapeComponent
	{
		IMPLEMENT_OBJECT(CapsuleShapeComponent, ShapeComponent);
	public:
		static void RegisterObject();

	public:
		CapsuleShapeComponent();
		~CapsuleShapeComponent();

		void createCapsule(float _fRadius, float _fHeight);

		void LoadXML(tinyxml2::XMLElement& element) override;
		void SaveXML(tinyxml2::XMLElement& element) override;

	private:
		void CreateShapeInternal() override;

	private:
		float m_fRadius;
		float m_fHeight;

	};



}

#endif // !SHAPESCOMPONENT_H
