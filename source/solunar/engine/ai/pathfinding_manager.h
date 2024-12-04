#ifndef SOLUNAR_PATHFINDING_MANAGER_H
#define SOLUNAR_PATHFINDING_MANAGER_H

namespace solunar
{
	class IPathfindingNavigationData;
	class IPathfindingSolver;
}

namespace solunar
{
	enum eSolverBackend 
	{
		kSolver_Astar,
		kSolver_Unknown=-1
	};

	enum eNavigationDataRepresentationType
	{
		kNavigationData_Graph,
		kNavigationData_Unknown=-1
	};


	class PathfindingManager : public Singleton<PathfindingManager>
	{
	public:
		PathfindingManager();
		~PathfindingManager();

		void Init(
			IPathfindingNavigationData* pDataImplBackend, 
			IPathfindingSolver* pSolver
		);

		void Shutdown();


	private:
		IPathfindingNavigationData* m_pNavigationData;
		IPathfindingSolver* m_pSolver;
	};

	extern PathfindingManager* g_aiPathfindingManager;
}

#endif