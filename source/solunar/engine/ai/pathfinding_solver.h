#ifndef SOLUNAR_AI_PATHFINDING_INTERFACE_H
#define SOLUNAR_AI_PATHFINDING_INTERFACE_H

namespace solunar
{
	class IPathfindingNavigationData;
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

	private:
	};

}

#endif