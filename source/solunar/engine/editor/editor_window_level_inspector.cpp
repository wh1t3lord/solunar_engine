#include "editor_window_level_inspector.h"
#include "imgui.h"
#include "graphics\light.h"
#include "../entity/world.h"
#include "editor_manager.h"

solunar::EditorWindow_LevelInspector::EditorWindow_LevelInspector() : m_show(false)
{
}

solunar::EditorWindow_LevelInspector::~EditorWindow_LevelInspector()
{
}

void solunar::EditorWindow_LevelInspector::Init(void)
{
}

void solunar::EditorWindow_LevelInspector::Draw(World* pWorld)
{
	Assert(pWorld && "must be valid!");
	Assert(g_editorManager && "must be valid!");

	if (ImGui::Begin("Level Inspector"))
	{
		/*
		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		static bool align_label_with_current_x_position = false;
		static bool test_drag_and_drop = false;

		// todo: kirrik -> make commands and command queue for processing these
	//	if (ImGui::Button("Create Light")) CreateLight();
	//	if (ImGui::Button("Create Wall")) CreateWall();

		if (align_label_with_current_x_position)
			ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

		// 'selection_mask' is dumb representation of what may be user-side selection state.
		//  You may retain selection state inside or outside your objects in whatever format you see fit.
		// 'node_clicked' is temporary storage of what node we have clicked to process selection at the end
		/// of the loop. May be a pointer to your own node type, etc.
		static int selection_mask = (1 << 2);
		int node_clicked = -1;
		const std::vector<Entity*>& entities = pWorld->GetEntityManager().GetEntities();

		for (int i = 0; i < entities.size(); i++)
		{
			Entity* entity = entities[i];
			// Disable the default "open on single-click behavior" + set Selected flag according to our selection.
			// To alter selection we use IsItemClicked() && !IsItemToggledOpen(), so clicking on an arrow doesn't alter selection.
			ImGuiTreeNodeFlags node_flags = base_flags;
			const bool is_selected = (selection_mask & (1 << i)) != 0;
			if (is_selected)
				node_flags |= ImGuiTreeNodeFlags_Selected;

#if 1
			bool isLight = !!entity->GetComponent<LightComponent>();
			if (isLight)
				ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Light 0x%p", entity);
			else
				ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Entity 0x%p", entity);
#endif
			// Items 0..2 are Tree Node

			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
				node_clicked = i;
			if (test_drag_and_drop && ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
				ImGui::Text("This is a drag and drop source");
				ImGui::EndDragDropSource();
			}
		}

		if (node_clicked != -1)
		{
			
			//g_SelectedEntity = entities[node_clicked];
			
			g_editorManager->SetSelectedEntity(entities[node_clicked]);

			// Update selection state
			// (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
			if (ImGui::GetIO().KeyCtrl)
				selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
			else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
				selection_mask = (1 << node_clicked);           // Click to single-select
		}
		if (align_label_with_current_x_position)
			ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
		//	ImGui::TreePop();
		*/

		if (ImGui::BeginTabBar("TabBar##LevelInspector"))
		{
			if (ImGui::BeginTabItem("All"))
			{
				const std::vector<Entity*>& entities = pWorld->GetEntityManager().GetEntities();

				for (int i = 0; i < entities.size(); i++)
				{
					Entity* entity = entities[i];

					char node_name[64];
					sprintf(node_name, sizeof(node_name), "%s##%p", entity->GetTypeInfo()->GetClassName(), entity);

					if (ImGui::TreeNode(node_name))
					{
						const auto& components = entity->GetAllComponents();

						for (Component* pComponent : components)
						{
							if (pComponent)
							{
								char component_name[64];
								sprintf(component_name, sizeof(component_name), "%s##%p", pComponent->GetTypeInfo()->GetClassName(), pComponent);

								if (ImGui::TreeNode(component_name))
								{


									ImGui::TreePop();
								}
							}
						}

						ImGui::TreePop();
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Light"))
			{
				const std::vector<Entity*>& entities = pWorld->GetEntityManager().GetEntities();

				for (int i = 0; i < entities.size(); i++)
				{
					Entity* entity = entities[i];


					LightComponent* pLight = entity->GetComponent<LightComponent>();

					if (pLight)
					{
						char node_name[64];
						sprintf(node_name, sizeof(node_name), "%s##%p", entity->GetTypeInfo()->GetClassName(), entity);

						if (ImGui::TreeNode(node_name))
						{
							const auto& components = entity->GetAllComponents();

							for (Component* pComponent : components)
							{
								if (pComponent)
								{
									char component_name[64];
									sprintf(component_name, sizeof(component_name), "%s##%p", pComponent->GetTypeInfo()->GetClassName(), pComponent);

									if (ImGui::TreeNode(component_name))
									{


										ImGui::TreePop();
									}
								}
							}

							ImGui::TreePop();
						}
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("AI"))
			{
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Sound"))
			{
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}

	ImGui::End();
}

const char* solunar::EditorWindow_LevelInspector::GetName(void) const
{
	return "Level Inspector";
}

bool solunar::EditorWindow_LevelInspector::IsShow(void) const
{
	return m_show;
}

void solunar::EditorWindow_LevelInspector::SetShow(bool status)
{
	m_show = status;
}

int solunar::EditorWindow_LevelInspector::GetEditingMode()
{
	return -1;
}

void solunar::EditorWindow_LevelInspector::UpdateEditingMode(InputManager* pInputManager, World* pWorld)
{

}
