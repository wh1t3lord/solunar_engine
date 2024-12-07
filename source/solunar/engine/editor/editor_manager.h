#ifndef EDITOR_MANAGER_H
#define EDITOR_MANAGER_H

namespace solunar
{
	class IEditorWindow;
	class World;
}

namespace solunar
{
	enum EditingMode
	{
		kEditingMode_AIGraphNavigation,
		kEditingMode_ObjectSelection,
		kEditingMode_NoSelection // unable to select anything on screen, but editor's GUI i working
	};

	class EditorManager : public Singleton<EditorManager>
	{
	public:
		EditorManager();
		~EditorManager();

		void Init(World* pWorld);
		void PostInit();
		void Shutdown();

		void Update();
		void RegisterWindow(IEditorWindow* pWindow);

		void* GetSelectedEntity();
		void SetSelectedEntity(void* pEntity);

		const std::vector<IEditorWindow*>& GetWindows(void) const;
		
		bool IsAINavigationEditingEnabled(void) const;
		void SetAINavigationEditingEnabled(bool value);

		bool IsObjectSelectionEnabled(void) const;
		void SetObjectSelectionEnabled(bool value);

	private:
		void InitWindows();
		void UpdateEditingModes();
		void UpdateEditingMode_AIGraphNavigation();
		void UpdateEditingMode_ObjectSelection();



	private:
		bool m_object_selection_enabled;
		bool m_ai_navigation_editing_enabled;

		EditingMode m_current_editing_mode;
		// todo: kirrik -> change to entity id type like uint32_t or something else, storing a pointer is not a good thing at all, it is temporary implementation
		void* m_pSelectedEntity;
		World* m_pWorld;
		std::vector<IEditorWindow*> m_windows;
	};
	
	extern EditorManager* g_editorManager;
}

#endif // EDITOR_MANAGER_H