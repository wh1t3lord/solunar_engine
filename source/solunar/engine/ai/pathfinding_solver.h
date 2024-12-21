#ifndef SOLUNAR_AI_PATHFINDING_INTERFACE_H
#define SOLUNAR_AI_PATHFINDING_INTERFACE_H

namespace solunar
{
	class IPathfindingNavigationData;
	enum eNavigationDataRepresentationType;
}

namespace solunar
{
	class IPathfindingSolver
	{
	public:
		virtual ~IPathfindingSolver(void) {}

		virtual void Init(IPathfindingNavigationData* pDataImpl) = 0;
		virtual void Shutdown() = 0;

		virtual void GetNearestPoint(
			unsigned char* pOutNodeID,
			unsigned char* pOutRegionID,
			float* pOutWorldPointPosition,
			unsigned char out_vec_size
		) = 0;

		// if returns -1 it means failed to obtain
		virtual int GetNearestPoint(
			Entity* pObject
		) = 0;

		virtual void BuildPathToTarget(
			Entity* pObject, 
			Entity* pTarget,
			std::vector<int>& nodes
		) = 0;

		virtual const glm::vec3& GetNodePosition(int node_id) const = 0;


	private:
	};

}

#endif