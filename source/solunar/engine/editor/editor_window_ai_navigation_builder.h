#ifndef SOLUNAR_EDITOR_AI_NAVIGATION_BUILDER_H
#define SOLUNAR_EDITOR_AI_NAVIGATION_BUILDER_H

#include "editor_window.h"

namespace solunar
{
	enum eNavigationType
	{
		kNavigationManualGraph,
		kNavigationAutoGrid,
		kNavigationNavMesh,
		kEndOfEnum
	};

	class EditorWindow_AINavigationBuilder : public IEditorWindow
	{
		struct BuilderConfig_ManualGraph
		{
			struct Node
			{
				unsigned char id;
				std::vector<unsigned char> neighbours;
			};

			bool was_changed_controls;
			unsigned char max_nodes_count;
			unsigned char max_regions_count;
			unsigned char max_nodes_neighbour_per_node_count;


			unsigned char real_max_neighbours_count_for_one_node;
			int total_nodes;
			int max_possible_total_nodes;
			size_t memory_allocation_after_compilation;

			// key=region|value=Node
			std::unordered_map<unsigned char, std::vector<Node>> nodes;
		};

	public:
		EditorWindow_AINavigationBuilder(void);
		~EditorWindow_AINavigationBuilder(void);

		void Init(void) override;
		void Draw(World* pWorld) override;
		const char* GetName(void) const override;
		bool IsShow(void) const override;
		void SetShow(bool status) override;

		int GetEditingMode() override;
		void UpdateEditingMode(InputManager* pInputManager, World* pWorld) override;
	private:
		void UpdateStats(BuilderConfig_ManualGraph& conf);
		void AddNodeOnSurface(const glm::vec3& world_pos);

	private:
		bool m_show;
		eNavigationType m_current_type;

		BuilderConfig_ManualGraph m_conf_mg;
	};

	const char* convert_enum_navtype_to_text(eNavigationType type);
}

#endif