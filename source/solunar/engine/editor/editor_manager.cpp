#include "editor_manager.h"
#include "editor_window.h"
#include "inputmanager.h"

namespace solunar
{
	EditorManager* g_editorManager = nullptr;

	EditorManager::EditorManager() :
		m_object_selection_enabled(false),
		m_ai_navigation_editing_enabled(false),
		m_current_editing_mode(EditingMode::kEditingMode_NoSelection),
		m_pWorld(nullptr),
		m_pSelectedEntity(nullptr)
	{
	}

	EditorManager::~EditorManager()
	{
		if (!m_windows.empty())
			Core::Msg("EditorManager::~EditorManager:: you forgot to clear (or even to delete data)!");

		Shutdown();
	}

	void EditorManager::Init(World* pWorld)
	{
		Assert(pWorld && "you must pass a valid world!");

		this->m_pWorld = pWorld;
	}

	void EditorManager::PostInit()
	{
		this->InitWindows();
	}

	void EditorManager::InitWindows()
	{
		for (IEditorWindow* pWindow : m_windows)
		{
			if (pWindow)
			{
				pWindow->Init();
			}
		}
	}

	void EditorManager::UpdateEditingModes()
	{
		if (this->m_ai_navigation_editing_enabled == false && this->m_object_selection_enabled == false)
		{
			if (this->m_current_editing_mode != EditingMode::kEditingMode_NoSelection)
			{
				this->m_current_editing_mode = EditingMode::kEditingMode_NoSelection;
			}
		}

		switch (this->m_current_editing_mode)
		{
		case EditingMode::kEditingMode_AIGraphNavigation:
		{
			this->UpdateEditingMode_AIGraphNavigation();



			break;
		}
		case EditingMode::kEditingMode_ObjectSelection:
		{
			this->UpdateEditingMode_ObjectSelection();




			break;
		}
		case EditingMode::kEditingMode_NoSelection:
		{
			// nothing it means user doesn't have ANY editing mode as ENABLED (true)
			// but we have to ensure that other programmers didn't break the logic

			Assert(this->m_ai_navigation_editing_enabled == false && this->m_object_selection_enabled == false);

			break;
		}
		}
	}

	void EditorManager::UpdateEditingMode_AIGraphNavigation()
	{
		Assert(this->m_current_editing_mode == EditingMode::kEditingMode_AIGraphNavigation && "you can't call this method if your editing mode is different!");

		auto* pInputManager = InputManager::GetInstance();

		if (pInputManager)
		{
			bool isLMBPressed = pInputManager->IsMouseButtonPressed(MouseButtons::MOUSE_BUTTON_LEFT);

			if (isLMBPressed)
			{

			}
		}
	}

	void EditorManager::UpdateEditingMode_ObjectSelection()
	{
		Assert(this->m_current_editing_mode == EditingMode::kEditingMode_ObjectSelection && "you can't call this method if your editing mode is different!");


	}

	void EditorManager::Shutdown()
	{
		for (IEditorWindow* pWindow : m_windows)
		{
			if (pWindow)
			{
				mem_delete(pWindow);
			}
		}

		m_windows.clear();
	}

	void EditorManager::Update()
	{
		for (IEditorWindow* pWindow : m_windows)
		{
			if (pWindow)
			{
				if (pWindow->IsShow())
				{
					pWindow->Draw(this->m_pWorld);
				}
			}
		}

		this->UpdateEditingModes();
	}

	void EditorManager::RegisterWindow(IEditorWindow* pWindow)
	{
		Assert(pWindow && "you must pass a valid window pointer!");

		m_windows.push_back(pWindow);
	}

	void* EditorManager::GetSelectedEntity()
	{
		return m_pSelectedEntity;
	}

	void EditorManager::SetSelectedEntity(void* pEntity)
	{
		m_pSelectedEntity = pEntity;
	}

	const std::vector<IEditorWindow*>& EditorManager::GetWindows(void) const
	{
		return m_windows;
	}

	bool EditorManager::IsAINavigationEditingEnabled(void) const
	{
		return m_ai_navigation_editing_enabled;
	}

	void EditorManager::SetAINavigationEditingEnabled(bool value)
	{
		m_ai_navigation_editing_enabled = value;

		if (value)
			this->m_current_editing_mode = EditingMode::kEditingMode_AIGraphNavigation;
	}

	bool EditorManager::IsObjectSelectionEnabled(void) const { return this->m_object_selection_enabled; }

	void EditorManager::SetObjectSelectionEnabled(bool value) { this->m_object_selection_enabled = value; if (value) this->m_current_editing_mode = EditingMode::kEditingMode_ObjectSelection; }
}