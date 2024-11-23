#ifndef ENGINE_ENTITY_WORLD_H
#define ENGINE_ENTITY_WORLD_H

#include "engine/entity/entitymanager.h"
#include "graphics/graphicsworld.h"

namespace solunar
{
	class IEditorWindow;
}

namespace solunar
{
	class PhysicsWorld;

	struct RayCastResult
	{
		Entity* m_entity;
		glm::vec3 m_hitPosition;
		glm::vec3 m_hitNormal;
	};

	// stores information for editor
	class EditorStateData
	{
	public:
		EditorStateData() : m_pSelectedEntity(nullptr) {}
		~EditorStateData() {}

		void* GetSelectedEntity() { return m_pSelectedEntity; }
		void SetSelectedEntity(void* pEntity) { m_pSelectedEntity = pEntity; }

	private:
		// todo: kirrik -> change to entity id type like uint32_t or something else, storing a pointer is not a good thing at all, it is temporary implementation
		void* m_pSelectedEntity;
	};

	class World : public SerializableObject
	{
	public:
		DECLARE_OBJECT(World);

	public:
		World();
		~World();

		void LoadXML(tinyxml2::XMLElement& element) override;
		void SaveXML(tinyxml2::XMLElement& element) override;

		void PostInitializeEntity(Entity* entity);

		void Update_PreEntityUpdate();
		void Update_LogicEntity();
		void Update_PhysicsEntity();
		void Update_Editor();

		Entity* CreateEntity();
		Entity* CreateEntityEx(const TypeInfo* typeInfo);

		EntityManager& GetEntityManager() { return m_entityManager; }
		GraphicsWorld* GetGraphicsWorld() { return m_graphicsWorld; }
		PhysicsWorld* GetPhysicsWorld() { return m_physicsWorld; }

		void TogglePhysicsDebugDraw();

		// Casting functions

		bool RayCast(RayCastResult& rayResult, const glm::vec3& rayStart, const glm::vec3& rayEnd, const int collisionFilter = -1);
		std::vector<Entity*> BoxCast(const glm::vec3& boxPos, const glm::vec3& boxSize);

		void RegisterEditorWindow(IEditorWindow* pWindow);

		EditorStateData* GetEditor();

	private:
		void Update_EditorWindows();

		std::vector<IEditorWindow*> m_editor_windows;
		EditorStateData m_editor;
	private:
		EntityManager m_entityManager;

		GraphicsWorld* m_graphicsWorld;
		PhysicsWorld* m_physicsWorld;
	};

}

#endif // !ENGINECORE_ENTITY_LEVEL_H
