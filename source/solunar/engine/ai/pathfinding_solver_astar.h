#ifndef SOLUNAR_AI_PATHFINDING_SOLVER_ASTAR_H
#define SOLUNAR_AI_PATHFINDING_SOLVER_ASTAR_H

#include "pathfinding_solver.h"

namespace solunar
{
	class PathfindingSolverAstar : public IPathfindingSolver
	{
	public:
		PathfindingSolverAstar(void);
		~PathfindingSolverAstar(void);

		void Init(IPathfindingNavigationData* pDataImpl) override;
		void Shutdown() override;

		void GetNearestPoint(
			unsigned char* pOutNodeID,
			unsigned char* pOutRegionID,
			float* pOutWorldPointPosition,
			unsigned char out_vec_size
		) override;

		// if returns -1 it means failed to obtain
		int GetNearestPoint(
			Entity* pObject
		) override;

		void BuildPathToTarget(
			Entity* pObject,
			Entity* pTarget,
			std::vector<int>& nodes
		) override;

		const glm::vec3& GetNodePosition(int node_id) const override;

	private:
		IPathfindingNavigationData* m_pNavigationData;
		// if GetNodePosition failed
		glm::vec3 m_def_pos;
	};
}

#endif