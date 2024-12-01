#include "editor_manager.h"
#include "editor_window.h"

namespace solunar
{
	EditorManager* g_editorManager = nullptr;

	EditorManager::EditorManager() :
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
				pWindow->Draw(this->m_pWorld);
			}
		}
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
}