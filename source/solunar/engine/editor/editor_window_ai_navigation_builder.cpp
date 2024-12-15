#include "editor_window_ai_navigation_builder.h"
#include "imgui.h"
#include "ai\pathfinding_navigation_graph.h"
#include "editor_manager.h"
#include "inputmanager.h"
#include "camera.h"
#include "entity/world.h"
#include "graphics\debugrenderer.h"
#include "entity/cameracomponent.h"
#include <random>


#undef max
#undef min

namespace solunar
{
	constexpr float _kMGLengthOfRoot = 0.5f;
	constexpr float _kMGLengthOfArrow = 0.25f;
	glm::vec3 _kMGSelectedNodeColor = glm::vec3(1.0f, 0.8f, 0.07f);

	EditorWindow_AINavigationBuilder::EditorWindow_AINavigationBuilder(void) : IEditorWindow(), m_show(false), m_current_type(eNavigationType::kNavigationManualGraph)
	{
		std::memset(&this->m_conf_mg, 1, sizeof(unsigned char) * 3 + sizeof(bool));
	}

	EditorWindow_AINavigationBuilder::~EditorWindow_AINavigationBuilder(void)
	{
	}

	void EditorWindow_AINavigationBuilder::Init(void)
	{
		this->m_current_type = eNavigationType::kNavigationManualGraph;

		this->InitConfig_ManualGraph(this->m_conf_mg);
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
			if (ImGui::Button("Clear"))
			{
				this->ClearNodes();
			}
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
				controls_was_changed = ImGui::SliderScalar("max nodes per region", ImGuiDataType_U8, &m_conf_mg.max_nodes_count, &_kMinSizeOfUnsignedChar, &_kMaxSizeOfUnsignedChar);

				if (controls_was_changed)
				{
					this->m_conf_mg.need_to_update_stats = true;
				}


				controls_was_changed = ImGui::SliderScalar("max neighbours per node", ImGuiDataType_U8, &m_conf_mg.max_nodes_neighbour_per_node_count, &_kMinSizeOfUnsignedChar, &_kMaxSizeOfUnsignedChar);

				if (controls_was_changed)
				{
					this->m_conf_mg.need_to_update_stats = true;
				}

				controls_was_changed = ImGui::SliderScalar("max regions", ImGuiDataType_U8, &m_conf_mg.max_regions_count, &_kMinSizeOfUnsignedChar, &_kMaxSizeOfUnsignedChar);

				if (controls_was_changed)
				{
					this->m_conf_mg.need_to_update_stats = true;
				}

				if (this->m_conf_mg.need_to_update_stats)
				{
					UpdateStats(this->m_conf_mg);

					this->m_conf_mg.need_to_update_stats = false;
				}


				ImGui::Text("Statistics:");

				ImGui::Text("\tMax possible nodes: %d", this->m_conf_mg.max_possible_total_nodes);
				ImGui::Text("\tCreated nodes: %d", this->m_conf_mg.total_nodes);
				ImGui::Text("\tMemory be used in game: %d bytes (%d Kb)", this->m_conf_mg.memory_allocation_after_compilation, this->m_conf_mg.memory_allocation_after_compilation / 1024);

				if (ImGui::CollapsingHeader("Editing"))
				{
					if (ImGui::Checkbox("Selecting Mode", &this->m_conf_mg.is_selecting_node_mode)) 
					{
						
					}

					ImGui::Text("Node Creation:");

					ImGui::Checkbox("One-direction (Selected->Created)", &this->m_conf_mg.node_creation_flag_onedirectional);

					if (this->m_conf_mg.node_creation_flag_onedirectional)
						this->m_conf_mg.node_creation_flag_bidirectional = false;

					ImGui::Checkbox("Bi-direction (Selected<->Created)", &this->m_conf_mg.node_creation_flag_bidirectional);

					if (this->m_conf_mg.node_creation_flag_bidirectional)
						this->m_conf_mg.node_creation_flag_onedirectional = false;
				}

				if (ImGui::CollapsingHeader("Details"))
				{
					for (const auto& pair : this->m_conf_mg.nodes)
					{
						ImGui::Text("\t\tRegion: %d", pair.first);
						ImGui::Text("\t\t\tNodes: %d", pair.second.size());

						char node_name[32];
						char button_move_to_name[16];
						char region_name[16];

						sprintf(region_name, sizeof(region_name), "Region - %d", pair.first);
						if (ImGui::CollapsingHeader(region_name))
						{
							for (const auto& node : pair.second)
							{
								sprintf(node_name, sizeof(node_name), "Node: %d (reg:%d)", node.id, pair.first);
								ImGui::Text(node_name);


								sprintf(button_move_to_name, sizeof(button_move_to_name), "Move To##%d", node.id);
								if (ImGui::Button(button_move_to_name))
								{
									// strage behaviour setting world pos but it is not correct position like it is expected 1:1
									CameraProxy::GetInstance()->GetCameraComponent()->GetEntity()->SetPosition(node.position);
								}

								ImGui::Separator();
							}
						}
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


		this->DrawDebug();
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

	int EditorWindow_AINavigationBuilder::GetEditingMode()
	{
		return EditingMode::kEditingMode_AIGraphNavigation;
	}

	void EditorWindow_AINavigationBuilder::UpdateEditingMode(InputManager* pInputManager, World* pWorld)
	{
		SHORT lbm_state = GetAsyncKeyState(VK_LBUTTON);

		this->UpdateSelectingNode();

		if (
			lbm_state & 0x01 && 
			!ImGui::IsAnyItemHovered() && 
			!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)
		)
		{
			Camera* pCamera = CameraProxy::GetInstance();

			if (pCamera)
			{
				const glm::vec2& mouse_pos = pInputManager->GetCursorPos();

				POINT cursor;
				GetCursorPos(&cursor);
				glm::vec3 ray_dir = CameraProxy::GetInstance()->GetScreenRay(cursor.x, cursor.y);

				RayCastResult rc_result;

				glm::vec3 cam_pos = CameraProxy::GetInstance()->GetPosition();
				if (pWorld->RayCast(rc_result, cam_pos, cam_pos + ray_dir * 1000.0f))
				{
					this->AddNodeOnSurface(
						rc_result.m_hitPosition,
						rc_result.m_hitNormal
					);
				}
			}
		}
	}

	void EditorWindow_AINavigationBuilder::UpdateStats(BuilderConfig_ManualGraph& conf)
	{
		if (conf.need_to_update_stats)
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

	void EditorWindow_AINavigationBuilder::AddNodeOnSurface(const glm::vec3& world_pos, const glm::vec3& normal)
	{
		switch (this->m_current_type)
		{
		case eNavigationType::kNavigationManualGraph:
		{
			if (this->m_conf_mg.is_selecting_node_mode)
				break;

#ifdef _DEBUG
			char buf[64];
			sprintf(buf, sizeof(buf), "Add node: %.2f %.2f %.2f Normal: %.2f %.2f %.2f \n",
				world_pos.x,
				world_pos.y,
				world_pos.z,
				normal.x,
				normal.y,
				normal.z
			);
			OutputDebugStringA(buf);
#endif

			if (this->m_conf_mg.selected_node.region_id != unsigned char(-1))
			{
				Assert(this->m_conf_mg.nodes.find(this->m_conf_mg.selected_node.region_id) != this->m_conf_mg.nodes.end() && "can't be because for selecting node you had to add node to storage!!! Something is broken");

				if (
					this->m_conf_mg.nodes[this->m_conf_mg.selected_node.region_id].size() < this->m_conf_mg.max_nodes_count)
				{
					BuilderConfig_ManualGraph::Node instance;
					instance.id = this->m_conf_mg.nodes[this->m_conf_mg.selected_node.region_id].size();
					instance.position = world_pos;
					this->m_conf_mg.nodes[this->m_conf_mg.selected_node.region_id].push_back(instance);
				}
			}
			else
			{
				unsigned char free_region = unsigned char(-1);

				for (unsigned char i = 0; i < 255; ++i)
				{
					if (i > this->m_conf_mg.max_regions_count)
						break;

					if (this->m_conf_mg.nodes.find(i) == this->m_conf_mg.nodes.end())
					{
						if (i < this->m_conf_mg.max_regions_count)
						{
							free_region = i;
							break;
						}
					}
					else
					{
						if (this->m_conf_mg.nodes[i].size() < this->m_conf_mg.max_nodes_count)
						{
							free_region = i;
							break;
						}
					}
				}

				if (free_region != unsigned char(-1))
				{
					BuilderConfig_ManualGraph::Node instance;
					instance.id = this->m_conf_mg.nodes[free_region].size();
					instance.position = world_pos;
					this->m_conf_mg.nodes[free_region].push_back(instance);
				}
			}



			this->m_conf_mg.need_to_update_stats = true;


			break;
		}
		default:
		{
			Assert(!"not implemented!");
			break;
		}
		}
	}

	void EditorWindow_AINavigationBuilder::UpdateCentersOfRegions()
	{

	}

	void EditorWindow_AINavigationBuilder::ClearNodes()
	{
		switch (this->m_current_type)
		{
		case eNavigationType::kNavigationManualGraph:
		{
			this->m_conf_mg.nodes.clear();
			this->m_conf_mg.need_to_update_stats = true;

#ifdef _DEBUG
			OutputDebugStringA("Cleared nodes!");
#endif

			break;
		}
		default:
		{
			Assert(!"Not implemented!");
			break;
		}
		}
	}

	void EditorWindow_AINavigationBuilder::DrawDebug()
	{
		switch (this->m_current_type)
		{
		case eNavigationType::kNavigationManualGraph:
		{
			for (const auto& region_and_nodes : this->m_conf_mg.nodes)
			{
				const std::vector<BuilderConfig_ManualGraph::Node>& nodes = region_and_nodes.second;

				for (const auto& node : nodes)
				{
					this->DrawDebugNode(region_and_nodes.first, node, node.position);
				}
			}

			break;
		}
		default:
		{
			Assert(!"not implemented");
			break;
		}
		}
	}

	void EditorWindow_AINavigationBuilder::DrawDebugNode(
		unsigned char region_id,
		const BuilderConfig_ManualGraph::Node& node,
		const glm::vec3& node_world_pos
	)
	{
		glm::vec3 color_for_region = this->m_conf_mg.m_debug_region_colors[region_id];

		if (node.selected)
			color_for_region = _kMGSelectedNodeColor;

		g_debugRender.DrawLine(node.position, node.position + glm::vec3(0.0f, _kMGLengthOfRoot, 0.0f), color_for_region);
		g_debugRender.DrawLine(node.position + glm::vec3(0.0f, _kMGLengthOfRoot, 0.0f), node.position + glm::vec3(_kMGLengthOfArrow, 0.0f, 0.0f), color_for_region);
		g_debugRender.DrawLine(node.position + glm::vec3(0.0f, _kMGLengthOfRoot, 0.0f), node.position + glm::vec3(-_kMGLengthOfArrow, 0.0f, 0.0f), color_for_region);
		g_debugRender.DrawLine(node.position + glm::vec3(0.0f, _kMGLengthOfRoot, 0.0f), node.position + glm::vec3(0.0f, 0.0f, _kMGLengthOfArrow), color_for_region);
		g_debugRender.DrawLine(node.position + glm::vec3(0.0f, _kMGLengthOfRoot, 0.0f), node.position + glm::vec3(0.0f, 0.0f, -_kMGLengthOfArrow), color_for_region);

	}

	void EditorWindow_AINavigationBuilder::InitConfig_ManualGraph(BuilderConfig_ManualGraph& config)
	{
		config.max_nodes_count = 16;
		config.max_regions_count = 4;
		config.max_nodes_neighbour_per_node_count = 4;
		config.max_possible_total_nodes = config.max_nodes_count * config.max_regions_count;
		config.need_to_update_stats = false;
		config.total_nodes = 0;
		config.memory_allocation_after_compilation = 0;


		config.selected_node.id = unsigned char(-1);
		config.selected_node.region_id = unsigned char(-1);
		config.node_collision_radius = _kMGLengthOfArrow;

		config.node_creation_flag_bidirectional = false;
		config.node_creation_flag_onedirectional = false;
		config.is_selecting_node_mode = false;

		config.pHoveredNode = nullptr;

		for (int i = 0; i < 255; ++i)
		{
			config.m_debug_region_colors[i].x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			config.m_debug_region_colors[i].y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			config.m_debug_region_colors[i].z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

			if (config.m_debug_region_colors[i] == _kMGSelectedNodeColor)
			{
				config.m_debug_region_colors[i].x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				config.m_debug_region_colors[i].y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				config.m_debug_region_colors[i].z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			}
		}
	}

	void EditorWindow_AINavigationBuilder::UpdateSelectingNode()
	{
		switch (this->m_current_type)
		{
		case eNavigationType::kNavigationManualGraph:
		{
			if (this->m_conf_mg.is_selecting_node_mode)
			{
				POINT cursor;
				GetCursorPos(&cursor);
				glm::vec3 ray_dir = CameraProxy::GetInstance()->GetScreenRay(cursor.x, cursor.y);
				glm::vec3 ray_begin = CameraProxy::GetInstance()->GetPosition();

				float t_min = std::numeric_limits<float>::max();
				BuilderConfig_ManualGraph::Node* pClosestNode = nullptr;
				for (auto& regionid_and_nodes : this->m_conf_mg.nodes)
				{
					auto& nodes = regionid_and_nodes.second;

					for (auto& node : nodes)
					{
						float t;
						glm::vec3 point_of_intersection;
						if (this->RayIntersectionSphereNode(
							ray_begin,
							ray_dir,
							node.position,
							this->m_conf_mg.node_collision_radius,
							t,
							point_of_intersection
						))
						{
							if (t < t_min)
							{
								pClosestNode = &node;
								t_min = t;
							}
						}
						else
						{
							if (node.selected)
								node.selected = false;
						}
					}
				}

				if (pClosestNode)
				{
					if (this->m_conf_mg.pHoveredNode)
						this->m_conf_mg.pHoveredNode->selected = false;

					this->m_conf_mg.pHoveredNode = pClosestNode;
					pClosestNode->selected = true;
				}
			}

			break;
		}
		default: 
		{
			Assert(!"not implemented!");
			break;
		}
		}
	}

	bool EditorWindow_AINavigationBuilder::RayIntersectionSphereNode(const glm::vec3& ray_origin, const glm::vec3& ray_dir, const glm::vec3& sphere_pos, float sphere_radius, float& t1, glm::vec3& point)
	{
		glm::vec3 m = ray_origin - sphere_pos;
		float b = glm::dot(m, ray_dir);
		float c = glm::dot(m, m) - sphere_radius * sphere_radius;

		// Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0) 
		if (c > 0.0f && b > 0.0f) 
			return false;

		float discr = b * b - c;

		// A negative discriminant corresponds to ray missing sphere 
		if (discr < 0.0f) return 0;

		// Ray now found to intersect sphere, compute smallest t value of intersection
		float t = -b - sqrt(discr);

		// If t is negative, ray started inside sphere so clamp t to zero 
		if (t < 0.0f) 
			t = 0.0f;

		point = ray_origin + t * ray_dir;
		t1 = t;
		return true;
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