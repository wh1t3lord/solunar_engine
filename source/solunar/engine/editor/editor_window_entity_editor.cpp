#include "editor_window_entity_editor.h"
#include "imgui.h"
#include "editor_manager.h"
#include "engine.h"

namespace solunar
{
	EditorWindow_EntityEditor::EditorWindow_EntityEditor() : m_show(false), m_selected_type(0)
	{
	}

	EditorWindow_EntityEditor::~EditorWindow_EntityEditor()
	{
	}

	void EditorWindow_EntityEditor::Init(void)
	{
		const auto& types = TypeManager::GetInstance()->GetAllTypes();

		Assert(types.empty() == false && "must be registered already here!!!!");
	}

	void EditorWindow_EntityEditor::Draw(World* pWorld)
	{
#ifdef _DEBUG
		if (g_engineData.m_editor)
		{
			Assert(g_editorManager && "must be valid!");
		}
#endif

		if (ImGui::Begin("Entity editor##Editor"))
		{
			Entity* pSelectedEntity = static_cast<Entity*>(g_editorManager->GetSelectedEntity());
			
			if (pSelectedEntity)
			{
				if (ImGui::BeginTabBar("Entity##Entity editor"))
				{
					if (ImGui::BeginTabItem("Components"))
					{

					//	ImGui::Combo("##EntityEditorComponents");

						if (ImGui::Button("Add component"))
						{
						}

						ImGui::EndTabItem();
					}

					ImGui::EndTabBar();
				}
			}
		}

		ImGui::End();
	}

	const char* EditorWindow_EntityEditor::GetName(void) const
	{
		return "Entity editor";
	}

	bool EditorWindow_EntityEditor::IsShow(void) const
	{
		return m_show;
	}

	void EditorWindow_EntityEditor::SetShow(bool status)
	{
		m_show = status;
	}
}