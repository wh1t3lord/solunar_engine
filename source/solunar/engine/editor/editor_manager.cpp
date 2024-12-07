#include "editor_manager.h"
#include "editor_window.h"
#include "inputmanager.h"
#include "engine.h"
#include "camera.h"
#include "entity\cameracomponent.h"

namespace solunar
{
	EditorManager* g_editorManager = nullptr;

	EditorManager::EditorManager() :
		m_object_selection_enabled(false),
		m_ai_navigation_editing_enabled(false),
		m_game_simulate(false),
		m_current_editing_mode(EditingMode::kEditingMode_NoSelection),
		m_pWorld(nullptr),
		m_pEditingMode_AINavigationGraph(nullptr),
		m_pEditingMode_ObjectSelection(nullptr),
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

		IEditorWindow* pWindow = GetWindowByEditingMode(EditingMode::kEditingMode_AIGraphNavigation);
		
		if (pWindow)
		{
			this->m_pEditingMode_AINavigationGraph = pWindow;
		}

		pWindow = GetWindowByEditingMode(EditingMode::kEditingMode_ObjectSelection);

		if (pWindow)
		{
			this->m_pEditingMode_ObjectSelection = pWindow;
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
		Assert(this->m_pEditingMode_AINavigationGraph && "you must obtain window at initialization stage");

		auto* pInputManager = InputManager::GetInstance();

		if (pInputManager)
		{
			if (this->m_pEditingMode_AINavigationGraph)
			{
				this->m_pEditingMode_AINavigationGraph->UpdateEditingMode(pInputManager, Engine::ms_world);
			}
		}
	}

	void EditorManager::UpdateEditingMode_ObjectSelection()
	{
		Assert(this->m_current_editing_mode == EditingMode::kEditingMode_ObjectSelection && "you can't call this method if your editing mode is different!");
		Assert(this->m_pEditingMode_ObjectSelection && "you must obtain window at initialization stage");

		auto* pInputManager = InputManager::GetInstance();

		if (pInputManager)
		{
			if (this->m_pEditingMode_ObjectSelection)
			{
				this->m_pEditingMode_ObjectSelection->UpdateEditingMode(pInputManager, Engine::ms_world);
			}
		}
	}

	void EditorManager::UpdateCamera()
	{
		if (!this->m_game_simulate)
		{
			CameraProxy* pCamera = CameraProxy::GetInstance();

			if (pCamera)
			{
				InputManager* pInputManager = InputManager::GetInstance();

				if (pInputManager)
				{
					if (pInputManager->IsPressed(KeyboardKeys::KEY_W))
					{
						glm::vec3 pos= pCamera->GetCameraComponent()->GetEntity()->GetPosition();
						pos += pCamera->GetDirection() * 0.02f;
						pCamera->GetCameraComponent()->GetEntity()->SetPosition(pos);
					}

					if (pInputManager->IsPressed(KeyboardKeys::KEY_S))
					{
						glm::vec3 pos = pCamera->GetCameraComponent()->GetEntity()->GetPosition();
						pos -= pCamera->GetDirection() * 0.02f;
						pCamera->GetCameraComponent()->GetEntity()->SetPosition(pos);
					}

					if (pInputManager->IsPressed(KeyboardKeys::KEY_D))
					{
						glm::vec3 pos = pCamera->GetCameraComponent()->GetEntity()->GetPosition();
						pos.x += 0.02f;
						pCamera->GetCameraComponent()->GetEntity()->SetPosition(pos);
					}

					if (pInputManager->IsPressed(KeyboardKeys::KEY_A))
					{
						glm::vec3 pos = pCamera->GetCameraComponent()->GetEntity()->GetPosition();
						pos.x -= 0.02f;
						pCamera->GetCameraComponent()->GetEntity()->SetPosition(pos);
					}

					if (pInputManager->IsMouseButtonPressed(MouseButtons::MOUSE_BUTTON_RIGHT))
					{
						g_engineData.m_shouldCaptureMouse = true;
						g_engineData.m_shouldHideMouse = true;
					}
					else
					{
						g_engineData.m_shouldCaptureMouse = false;
						g_engineData.m_shouldHideMouse = false;

						// todo: typo ne ok, because if you handle input by g_engineData thus
						// you have to debug your state and disable/enable because InputManager
						// it is not right to manually call it here if you comment last two lines of code
						// the cursor will hide and only alt/tab will reveive it and it is not okay :(
						InputManager::GetInstance()->SetCursorHiding(false);
						InputManager::GetInstance()->SetCursorCapture(false);
					}
				}
			}
		}
	}

	IEditorWindow* EditorManager::GetWindowByEditingMode(EditingMode mode)
	{
		IEditorWindow* pResult = 0;

		auto it = std::find_if(this->m_windows.begin(), this->m_windows.end(), [mode](IEditorWindow* pWindow) {
			if (!pWindow)
				return false;

			return static_cast<EditingMode>(pWindow->GetEditingMode()) == mode;
		});

		if (it != this->m_windows.end())
			pResult = *it;

		return pResult;
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

		this->UpdateCamera();
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

	bool EditorManager::IsSimulate(void) const
	{
		return this->m_game_simulate;
	}

	void EditorManager::SetSimulate(bool value)
	{
		this->m_game_simulate = value;

		this->m_current_editing_mode = EditingMode::kEditingMode_NoSelection;
		this->m_ai_navigation_editing_enabled = false;
		this->m_object_selection_enabled = false;
	}
}