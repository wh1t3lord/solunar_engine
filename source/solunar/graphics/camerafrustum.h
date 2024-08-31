#ifndef CAMERAFRUSTUM_H
#define CAMERAFRUSTUM_H

#include "core/math/boundingbox.h"

#define CAMERA_FRUSTUM_PLANE_COUNT 6

namespace solunar
{
	enum Halfspace
	{
		Halfspace_Negative = -1,
		Halfspace_OnPlane = 0,
		Halfspace_Positive = 1
	};

	struct Plane
	{
		float a, b, c;
		float d;

		void SetIdentity();
		
		Plane& normalize();

		float distanceToPoint(const glm::vec3& pt);

		Halfspace classifyPoint(const glm::vec3& pt);
	};

	class CameraFrustum
	{
	public:
		void Update(glm::mat4& vp);

		bool isBoundingBoxInside(const BoundingBox& boundingBox);

	private:
		Plane m_planes[CAMERA_FRUSTUM_PLANE_COUNT];
	};
}

#endif // !CAMERAFRUSTUM_H
