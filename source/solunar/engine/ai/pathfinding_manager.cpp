#include "pathfinding_navigation.h"
#include "pathfinding_solver.h"
#include "pathfinding_manager.h"

namespace solunar
{
	PathfindingManager* g_aiPathfindingManager = nullptr;

	PathfindingManager::PathfindingManager() : m_enable_debug_draw(false), m_pNavigationData(nullptr), m_pSolver(nullptr)
	{
	}

	PathfindingManager::~PathfindingManager()
	{
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
	void PathfindingManager::ToggleDebugDraw()
	{
		this->m_enable_debug_draw = !this->m_enable_debug_draw;
	}
	void PathfindingManager::DebugDraw()
	{
		if (this->m_enable_debug_draw)
		{
			if (this->m_pNavigationData)
			{
				this->m_pNavigationData->DebugDraw();
			}
		}
	}
}