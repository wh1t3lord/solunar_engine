#ifndef SHAPESCOMPONENT_H
#define SHAPESCOMPONENT_H

#include "engine/physics/bullet_private.h"
#include "engine/physics/rigidbodycomponent.h"

namespace engine {

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
	class ENGINE_API ShapeComponent : public Component
	{
		ImplementObject(ShapeComponent, Component);
	public:
		static void registerObject();

	public:
		ShapeComponent();
		virtual ~ShapeComponent();

		void onEntitySet(Entity* entity) override;
		void onEntityRemove() override;
		void onWorldSet(World* world) override;

		// serialization
		virtual void loadXML(tinyxml2::XMLElement& element);
		virtual void saveXML(tinyxml2::XMLElement& element);

		//! Public initialization of shape
		void initializeShape();

		//! Get Physics SDK shape.
		btCollisionShape* getSdkShape() { return m_shape; }

	private:
		void initializeShapeTransform();

	protected:
		//! Base virtual function for shape creation.
		virtual void createShapeInternal();

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

	class ENGINE_API BoxShapeComponent : public ShapeComponent
	{
		ImplementObject(BoxShapeComponent, ShapeComponent);
	public:
		static void registerObject();

	public:
		BoxShapeComponent();
		~BoxShapeComponent();

		void loadXML(tinyxml2::XMLElement& element) override;
		void saveXML(tinyxml2::XMLElement& element) override;

		void createShape(const glm::vec3& size);

	private:
		void createShapeInternal() override;

	private:
		glm::vec3 m_size;
	};

	class ENGINE_API SphereShapeComponent : public ShapeComponent
	{
		ImplementObject(SphereShapeComponent, ShapeComponent);
	public:
		static void registerObject();

	public:
		SphereShapeComponent();
		~SphereShapeComponent();

		void loadXML(tinyxml2::XMLElement& element) override;
		void saveXML(tinyxml2::XMLElement& element) override;

	private:
		void createShapeInternal() override;

	private:
		float m_fRadius;

	};

	class ENGINE_API CylinderShapeComponent : public ShapeComponent
	{
		ImplementObject(CylinderShapeComponent, ShapeComponent);
	public:
		static void registerObject();

	public:
		CylinderShapeComponent();
		~CylinderShapeComponent();

		void loadXML(tinyxml2::XMLElement& element) override;
		void saveXML(tinyxml2::XMLElement& element) override;

	private:
		void createShapeInternal() override;

	private:
		glm::vec3 m_size;

	};

	class ENGINE_API CapsuleShapeComponent : public ShapeComponent
	{
		ImplementObject(CapsuleShapeComponent, ShapeComponent);
	public:
		static void registerObject();

	public:
		CapsuleShapeComponent();
		~CapsuleShapeComponent();

		void loadXML(tinyxml2::XMLElement& element) override;
		void saveXML(tinyxml2::XMLElement& element) override;

	private:
		void createShapeInternal() override;

	private:
		float m_fRadius;
		float m_fHeight;

	};



}

#endif // !SHAPESCOMPONENT_H
