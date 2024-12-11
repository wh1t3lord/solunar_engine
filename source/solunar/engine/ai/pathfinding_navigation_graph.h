#ifndef SOLUNAR_AI_PATHFINDING_NAVIGATION_GRAPH_H
#define SOLUNAR_AI_PATHFINDING_NAVIGATION_GRAPH_H

#include "pathfinding_navigation.h"

namespace solunar
{
	struct PathfindingNavigationStaticCachedGraphNode
	{
		unsigned char id;
		unsigned char region_id;
	};

	// todo: provide implementation for streaming graph from file for HUGE worlds
	// name it PathfindingNavigationStreamingGraph (if it is needed)

	/*
		binary file (after compilation):
			
			HEADER SECTION

			name															type

			total_nodes_count=nodes_per_region_count*total_regions_count,	unsigned char
			nodes_per_region_count,											unsigned char
			total_regions_count,											unsigned char
			calculated_max_neighbours_in_all_nodes,							unsigned char
			calculated_memory_for_allocation,								size_t

			NODES SECTION	(total_nodes_count * sizeof(PathfindingNavigationStaticCachedGraphNode))

			node_id (unsigned char), region_id (unsigned char)	
			0						0
			1						0
			2						0
			3						0
			4						0
			...

			NEIGBOURS SECTION (total_nodes_count * calculated_max_neighbours_in_all_nodes)
			
			since it is a look-up table so it is sorted by ids

		0:	neighbours_count,	unsigned char
			neighbour			unsigned char
			neighbour			unsigned char
			neighbour			unsigned char
			...

		1: neighbours_count,	unsigned char
			neighbour			unsigned char
			neighbour			unsigned char
			neighbour			unsigned char
			...

			REGION CENTERS, size=(max_region_count * sizeof(float)*3)

		1:	x	float
			y	float
			z	float

			NODES POSITIONS, size=(total_nodes_count * sizeof(float)*3)

		0:	x	float
			y	float
			z	float


		total memory for allocation = NODES POSITIONS + REGION CENTERS + NEIGBOURS SECTION + NODES SECTION 

		for example:
			total_nodes_count=720
			total_regions_count=10
			max_neighbours=16

			NODES POSITIONS = 720 * 12 = 8640 bytes
			REGION CENTERS = 10 * 12 = 120 bytes
			NEIGBOURS SECTION = 720 * 16 = 11520 bytes (because one node can have 16 nodes as neighbours)
			NODES SECTION = 720 * 2 = 1440 bytes

			total memory = 21720 bytes

			So world positions it is pointless to pack into long long, so just store as is. 

			But neighbours it is vital to keep neighbours max size minimal!

			You can specify in graph builder settings:
				- max nodes per region count  
				- max neighbours per node count
				- max region count
	*/
	class PathfindingNavigationStaticCachedGraph : IPathfindingNavigationData
	{
	public:
		PathfindingNavigationStaticCachedGraph(void);
		~PathfindingNavigationStaticCachedGraph(void);

		void Init();
		void Load(const char* pFullPathToFile);
	
	private:
		unsigned char m_region_count;
		unsigned char m_nodes_per_region_count;
		unsigned char m_max_neighbours_per_node;
		float* m_pWorldPositionCenterOfRegions;  
		PathfindingNavigationStaticCachedGraphNode* m_pNodes;  
		unsigned char** m_pNeighbours; // look-up table
		float* m_pWorldPositionOfNodes;
	};
}

#endif