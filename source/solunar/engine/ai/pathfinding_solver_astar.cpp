#include "pathfinding_solver_astar.h"
#include "pathfinding_navigation_graph.h"

namespace solunar
{
	PathfindingSolverAstar::PathfindingSolverAstar(void) : m_pNavigationData(nullptr), m_def_pos(0.0f,0.0f,0.0f)
	{
	}

	PathfindingSolverAstar::~PathfindingSolverAstar(void)
	{
	}

	void PathfindingSolverAstar::Init(IPathfindingNavigationData* pDataImpl)
	{
		Assert(pDataImpl && "must be valid");
		this->m_pNavigationData = pDataImpl;
	}

	void PathfindingSolverAstar::Shutdown()
	{
	}

	void PathfindingSolverAstar::GetNearestPoint(unsigned char* pOutNodeID, unsigned char* pOutRegionID, float* pOutWorldPointPosition, unsigned char out_vec_size)
	{
		Assert(!"Not implemented");
	}

	int PathfindingSolverAstar::GetNearestPoint(Entity* pObject)
	{
		Assert(this->m_pNavigationData && "early calling, initialize this before call this");
		Assert(pObject && "you passed an invalid object");

		int result = -1;

		if (!this->m_pNavigationData)
			return result;
		
		if (pObject)
		{
			switch (this->m_pNavigationData->GetType())
			{
			case eNavigationDataRepresentationType::kNavigationData_Graph_XML:
			{
				PathfindingNavigationStaticGraph* pCasted = static_cast<PathfindingNavigationStaticGraph*>(this->m_pNavigationData);

				if (pCasted)
				{

				}
				
				break;
			}
			default:
			{
				Assert(!"not implemeneted yet!");
				break;
			}
			}
		}

		return result;
	}

	void PathfindingSolverAstar::BuildPathToTarget(Entity* pObject, Entity* pTarget, std::vector<int>& nodes)
	{
	}

	const glm::vec3& PathfindingSolverAstar::GetNodePosition(int node_id) const
	{
		return this->m_def_pos;
	}
}