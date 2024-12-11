#include "pathfinding_navigation.h"
#include "pathfinding_solver.h"
#include "pathfinding_manager.h"

namespace solunar
{
	PathfindingManager* g_aiPathfindingManager = nullptr;

	PathfindingManager::PathfindingManager() : m_pNavigationData(nullptr), m_pSolver(nullptr)
	{
	}

	PathfindingManager::~PathfindingManager()
	{
		Assert(m_pNavigationData && "memory leak, you forgot to delete it in ::Shutdown!");
		Assert(m_pSolver && "memory leak, you forgot to delete it in ::Shutdown!");
	}

	void PathfindingManager::Init(IPathfindingNavigationData* pDataImplBackend, IPathfindingSolver* pSolver)
	{
		Assert(pDataImplBackend && "you must pass a valid implementation, just allocate the needed class implementation of IPathfindingNavigationData interface");
		Assert(pSolver && "you must pass a valid implementation, just allocate the needed class implementation of IPathfindingSolver");

		this->m_pNavigationData = pDataImplBackend;
		this->m_pSolver = pSolver;
	}

	void PathfindingManager::Shutdown()
	{
		if (m_pNavigationData)
		{
			mem_delete(m_pNavigationData);
			m_pNavigationData = nullptr;
		}

		if (m_pSolver)
		{
			mem_delete(m_pSolver);
			m_pSolver = nullptr;
		}
	}
}