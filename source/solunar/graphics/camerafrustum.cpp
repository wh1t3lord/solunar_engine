#include "graphicspch.h"
#include "graphics/camerafrustum.h"

namespace solunar
{
	// PLANE IMPLEMENTATION

	void Plane::SetIdentity()
	{
		a = 0.0;
		b = 0.0f;
		c = 0.0f;
		d = 0.0f;
	}

	Plane& Plane::normalize()
	{
		float mag = sqrt(a * a + b * b + c * c);
		a = a / mag;
		b = b / mag;
		c = c / mag;
		d = d / mag;
		return *this;
	}

	float Plane::distanceToPoint(const glm::vec3& pt)
	{
		return a * pt.x + b * pt.y + c * pt.z + d;
	}

	Halfspace Plane::classifyPoint(const glm::vec3& pt)
	{
		float d = a * pt.x + b * pt.y + c * pt.z + d;
		if (d < 0) return Halfspace_Negative;
		if (d > 0) return Halfspace_Positive;
		return Halfspace_OnPlane;
	}

	// CAMERAFRUSTUM IMPLEMENTATION

	void CameraFrustum::Update(glm::mat4& vp)
	{
		// extracting frustum from combined matrix ( proj * view )
		//glm::mat4 comboMatrix = glm::transpose(vp);
		glm::mat4 comboMatrix = vp;

		// Left clipping plane
		m_planes[0].a = comboMatrix[3][0] + comboMatrix[0][0];
		m_planes[0].b = comboMatrix[3][1] + comboMatrix[0][1];
		m_planes[0].c = comboMatrix[3][2] + comboMatrix[0][2];
		m_planes[0].d = comboMatrix[3][3] + comboMatrix[0][3];

		// Right clipping plane
		m_planes[1].a = comboMatrix[3][0] - comboMatrix[0][0];
		m_planes[1].b = comboMatrix[3][1] - comboMatrix[0][1];
		m_planes[1].c = comboMatrix[3][2] - comboMatrix[0][2];
		m_planes[1].d = comboMatrix[3][3] - comboMatrix[0][3];

		// Top clipping plane
		m_planes[2].a = comboMatrix[3][0] - comboMatrix[1][0];
		m_planes[2].b = comboMatrix[3][1] - comboMatrix[1][1];
		m_planes[2].c = comboMatrix[3][2] - comboMatrix[1][2];
		m_planes[2].d = comboMatrix[3][3] - comboMatrix[1][3]; 

		// Bottom clipping plane
		m_planes[3].a = comboMatrix[3][0] + comboMatrix[1][0];
		m_planes[3].b = comboMatrix[3][1] + comboMatrix[1][1];
		m_planes[3].c = comboMatrix[3][2] + comboMatrix[1][2];
		m_planes[3].d = comboMatrix[3][3] + comboMatrix[1][3];

		// Near clipping plane
		m_planes[4].a = comboMatrix[3][0] + comboMatrix[2][0];
		m_planes[4].b = comboMatrix[3][1] + comboMatrix[2][1];
		m_planes[4].c = comboMatrix[3][2] + comboMatrix[2][2];
		m_planes[4].d = comboMatrix[3][3] + comboMatrix[2][3];

		// Far clipping plane
		m_planes[5].a = comboMatrix[3][0] - comboMatrix[2][0];
		m_planes[5].b = comboMatrix[3][1] - comboMatrix[2][1];
		m_planes[5].c = comboMatrix[3][2] - comboMatrix[2][2];
		m_planes[5].d = comboMatrix[3][3] - comboMatrix[2][3];
	
		bool normalize = false;
		if (normalize)
		{
			for (int i = 0; i < CAMERA_FRUSTUM_PLANE_COUNT; i++)
			{
				m_planes[i].normalize();
			}
		}
	}

	bool CameraFrustum::isBoundingBoxInside(const BoundingBox& boundingBox)
	{
		bool result = true;
		glm::vec3 vmin = glm::vec3(0.0f), vmax = glm::vec3(0.0f);

		for (int i = 0; i < 6; ++i)
		{
			// X axis 
			if (m_planes[i].a > 0)
			{
				vmin.x = boundingBox.m_min.x;
				vmax.x = boundingBox.m_max.x;
			}
			else
			{
				vmin.x = boundingBox.m_max.x;
				vmax.x = boundingBox.m_min.x;
			}
			// Y axis 
			if (m_planes[i].b > 0)
			{
				vmin.y = boundingBox.m_min.y;
				vmax.y = boundingBox.m_max.y;
			}
			else
			{
				vmin.y = boundingBox.m_max.y;
				vmax.y = boundingBox.m_min.y;
			}
			// Z axis 
			if (m_planes[i].c > 0)
			{
				vmin.z = boundingBox.m_min.z;
				vmax.z = boundingBox.m_max.z;
			}
			else
			{
				vmin.z = boundingBox.m_max.z;
				vmax.z = boundingBox.m_min.z;
			}

			if (glm::dot(glm::vec3(m_planes[i].a, m_planes[i].b, m_planes[i].c), vmin) + m_planes[i].d >= 0)
				return false;
		}

		return result;
	}
}
