#ifndef CAMERAFRUSTUM_H
#define CAMERAFRUSTUM_H

#include "core/math/boundingbox.h"

namespace engine
{
	struct Plane
	{
		float a, b, c;
		float d;
	};

	class CameraFrustum
	{
	public:
		void update(glm::mat4& vp);

		bool isBoundingBoxInside(const BoundingBox& boundingBox);

	private:
		Plane m_plane[6];
	};
}

#endif // !CAMERAFRUSTUM_H
