#ifndef EDITOR_MANAGER_H
#define EDITOR_MANAGER_H

namespace solunar
{
	class IEditorWindow;
	class World;
}

namespace solunar
{
	class EditorManager : public Singleton<EditorManager>
	{
	public:
		EditorManager();
		~EditorManager();

		void Init(World* pWorld);
		void Shutdown();

		void Update();
		void RegisterWindow(IEditorWindow* pWindow);

		void* GetSelectedEntity();
		void SetSelectedEntity(void* pEntity);

		const std::vector<IEditorWindow*>& GetWindows(void) const;
	private:
		// todo: kirrik -> change to entity id type like uint32_t or something else, storing a pointer is not a good thing at all, it is temporary implementation
		void* m_pSelectedEntity;
		World* m_pWorld;
		std::vector<IEditorWindow*> m_windows;
	};
	
	extern EditorManager* g_editorManager;
}

#endif // EDITOR_MANAGER_H