#include "editor_window_ai_navigation_builder.h"
#include "imgui.h"
#include "ai\pathfinding_navigation_graph.h"
#include "editor_manager.h"

#undef max
#undef min

namespace solunar
{
	EditorWindow_AINavigationBuilder::EditorWindow_AINavigationBuilder(void) : IEditorWindow(), m_show(false), m_current_type(eNavigationType::kNavigationManualGraph)
	{
		std::memset(&this->m_conf_mg, 1, sizeof(unsigned char)*3 + sizeof(bool));
	}

	EditorWindow_AINavigationBuilder::~EditorWindow_AINavigationBuilder(void)
	{
	}

	void EditorWindow_AINavigationBuilder::Init(void)
	{
		this->m_current_type = eNavigationType::kNavigationManualGraph;

		this->m_conf_mg.max_possible_total_nodes = this->m_conf_mg.max_nodes_count * this->m_conf_mg.max_regions_count;
		this->m_conf_mg.was_changed_controls = false;
		this->m_conf_mg.total_nodes = 0;
	}

	void EditorWindow_AINavigationBuilder::Draw(World* pWorld)
	{
		if (ImGui::Begin("[AI] - Navigation Builder"))
		{
			bool is_editing = g_editorManager->IsAINavigationEditingEnabled();
			ImGui::Checkbox("Graph Editing", &is_editing);
			g_editorManager->SetAINavigationEditingEnabled(is_editing);

			ImGui::SameLine();
			ImGui::Button("Build");
			ImGui::SameLine();
			ImGui::Button("Clear");
			ImGui::Separator();

			const char* combobox_navtypes_labels[eNavigationType::kEndOfEnum];

			for (unsigned char i = 0; i < static_cast<unsigned char>(eNavigationType::kEndOfEnum); ++i)
			{
				combobox_navtypes_labels[i] = convert_enum_navtype_to_text(static_cast<eNavigationType>(i));
			}

			int selected_navtype = static_cast<int>(this->m_current_type);
			ImGui::Combo("Navigation type", &selected_navtype, combobox_navtypes_labels, static_cast<int>(eNavigationType::kEndOfEnum));
			ImGui::Separator();
 
			switch (this->m_current_type)
			{
			case eNavigationType::kNavigationManualGraph:
			{
				constexpr unsigned char _kMaxSizeOfUnsignedChar = std::numeric_limits<unsigned char>::max();
				constexpr unsigned char _kMinSizeOfUnsignedChar = 1;
				
				bool controls_was_changed = false;
				controls_was_changed = ImGui::SliderScalar("max nodes", ImGuiDataType_U8, &m_conf_mg.max_nodes_count, &_kMinSizeOfUnsignedChar, &_kMaxSizeOfUnsignedChar);
				
				if (controls_was_changed)
				{
					this->m_conf_mg.was_changed_controls = true;
				}


				controls_was_changed = ImGui::SliderScalar("max neighbours per node", ImGuiDataType_U8,&m_conf_mg.max_nodes_neighbour_per_node_count, &_kMinSizeOfUnsignedChar, &_kMaxSizeOfUnsignedChar);
				
				if (controls_was_changed)
				{
					this->m_conf_mg.was_changed_controls = true;
				}

				controls_was_changed = ImGui::SliderScalar("max regions", ImGuiDataType_U8, &m_conf_mg.max_regions_count, &_kMinSizeOfUnsignedChar, &_kMaxSizeOfUnsignedChar);

				if (controls_was_changed)
				{
					this->m_conf_mg.was_changed_controls = true;
				}

				if (this->m_conf_mg.was_changed_controls)
				{
					UpdateStats(this->m_conf_mg);

					this->m_conf_mg.was_changed_controls = false;
				}


				ImGui::Text("Statistics:");

				ImGui::Text("\tMax possible nodes: %d", this->m_conf_mg.max_possible_total_nodes);
				ImGui::Text("\tCreated nodes: %d", this->m_conf_mg.total_nodes);
				ImGui::Text("\tMemory be used in game: %d (bytes; %d Kb)", this->m_conf_mg.memory_allocation_after_compilation, this->m_conf_mg.memory_allocation_after_compilation/1024);

				if (ImGui::CollapsingHeader("Details"))
				{
					for (const auto& pair : this->m_conf_mg.nodes)
					{
						ImGui::Text("\t\tRegion: %d", pair.first);
						ImGui::Text("\t\t\tNodes: %d", pair.second.size());
					}
				}


				break;
			}
			case eNavigationType::kNavigationAutoGrid:
			{
				Core::Msg("[WARNING] navigation type: [%s] not implemented", convert_enum_navtype_to_text(this->m_current_type));
				this->m_current_type = eNavigationType::kNavigationManualGraph;
				break;
			}
			case eNavigationType::kNavigationNavMesh:
			{
				Core::Msg("[WARNING] navigation type: [%s] not implemented", convert_enum_navtype_to_text(this->m_current_type));
				this->m_current_type = eNavigationType::kNavigationManualGraph;
				break;
			}
			default:
			{
				Assert(false && "unreachable code");
				break;
			}
			}
		}

		ImGui::End();
	}

	const char* EditorWindow_AINavigationBuilder::GetName(void) const
	{
		return "AI Navigation Builder";
	}

	bool EditorWindow_AINavigationBuilder::IsShow(void) const
	{
		return m_show;
	}

	void EditorWindow_AINavigationBuilder::SetShow(bool status)
	{
		m_show = status;
	}

	void EditorWindow_AINavigationBuilder::UpdateStats(BuilderConfig_ManualGraph& conf)
	{
		if (conf.was_changed_controls)
		{
			conf.max_possible_total_nodes = conf.max_nodes_count * conf.max_regions_count;
			conf.memory_allocation_after_compilation = 0;
			conf.total_nodes = 0;

			unsigned char max_neighbours_count = 0;
			for (const auto& pair : conf.nodes)
			{
				for (const BuilderConfig_ManualGraph::Node& node : pair.second)
				{
					size_t neighbours_count = node.neighbours.size();
					if (neighbours_count > max_neighbours_count)
					{
						max_neighbours_count = static_cast<unsigned char>(neighbours_count);
					}
				}

				this->m_conf_mg.total_nodes += pair.second.size();
			}

			conf.real_max_neighbours_count_for_one_node = max_neighbours_count;

			// NODES SECTION 

			conf.memory_allocation_after_compilation += conf.max_possible_total_nodes * sizeof(PathfindingNavigationStaticCachedGraphNode);

			// NEIGHBOURS SECTION

			conf.memory_allocation_after_compilation += conf.max_possible_total_nodes * conf.real_max_neighbours_count_for_one_node;

			// REGION CENTERS

			conf.memory_allocation_after_compilation += conf.max_regions_count * sizeof(float) * 3;

			// NODES POSITIONS

			conf.memory_allocation_after_compilation += conf.max_possible_total_nodes * sizeof(float) * 3;

		}
	}

	const char* convert_enum_navtype_to_text(eNavigationType type)
	{
		switch (type)
		{
		case eNavigationType::kNavigationManualGraph:
		{
			return "Manual Graph";
		}
		case eNavigationType::kNavigationAutoGrid:
		{
			return "Auto Graph";
		}
		case eNavigationType::kNavigationNavMesh:
		{
			return "Navigation Mesh";
		}
		default:
			return "UNDEFINED_NAVIGATION_TYPE";
		}
	}
}