#include "graphicspch.h"
#include "graphics/camerafrustum.h"

namespace engine
{
	void CameraFrustum::update(glm::mat4& vp)
	{
		// extracting frustum from combined matrix ( proj * view )
		glm::mat4 comboMatrix = glm::transpose(vp);

		/*// Left clipping plane
		m_plane[0].a = comboMatrix[3][0] + comboMatrix[0][0];
		m_plane[0].b = comboMatrix[3][1] + comboMatrix[0][1];
		m_plane[0].c = comboMatrix[3][2] + comboMatrix[0][2];
		m_plane[0].d = comboMatrix[3][3] + comboMatrix[0][3];
		
		// Right clipping plane
		m_plane[1].a = comboMatrix[3][1] - comboMatrix[1][1];
		m_plane[1].b = comboMatrix[3][2] - comboMatrix[1][2];
		m_plane[1].c = comboMatrix[3][3] - comboMatrix[1][3];
		m_plane[1].d = comboMatrix[3][4] - comboMatrix[1][4];
		
		// Top clipping plane
		m_plane[2].a = comboMatrix[3][1] - comboMatrix[2][1];
		m_plane[2].b = comboMatrix[3][2] - comboMatrix[2][2];
		m_plane[2].c = comboMatrix[3][3] - comboMatrix[2][3];
		m_plane[2].d = comboMatrix[3][4] - comboMatrix[2][4];
		
		// Bottom clipping plane
		m_plane[3].a = comboMatrix[3][1] + comboMatrix[2][1];
		m_plane[3].b = comboMatrix[3][2] + comboMatrix[2][2];
		m_plane[3].c = comboMatrix[3][3] + comboMatrix[2][3];
		m_plane[3].d = comboMatrix[3][4] + comboMatrix[2][4];
		
		// Near clipping plane
		m_plane[4].a = comboMatrix[3][1] + comboMatrix[3][1];
		m_plane[4].b = comboMatrix[3][2] + comboMatrix[3][2];
		m_plane[4].c = comboMatrix[3][3] + comboMatrix[3][3];
		m_plane[4].d = comboMatrix[3][4] + comboMatrix[3][4];
		
		// Far clipping plane
		m_plane[5].a = comboMatrix[4][1] - comboMatrix[3][1];
		m_plane[5].b = comboMatrix[4][2] - comboMatrix[3][2];
		m_plane[5].c = comboMatrix[4][3] - comboMatrix[3][3];
		m_plane[5].d = comboMatrix[4][4] - comboMatrix[3][4];*/
	}

	bool CameraFrustum::isBoundingBoxInside(const BoundingBox& boundingBox)
	{
		bool result = true;
		glm::vec3 vmin, vmax;

		for (int i = 0; i < 6; ++i)
		{
			// X axis 
			if (m_plane[i].a > 0)
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
			if (m_plane[i].b > 0)
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
			if (m_plane[i].c > 0)
			{
				vmin.z = boundingBox.m_min.z;
				vmax.z = boundingBox.m_max.z;
			}
			else
			{
				vmin.z = boundingBox.m_max.z;
				vmax.z = boundingBox.m_min.z;
			}

			if (glm::dot(glm::vec3(m_plane[i].a, m_plane[i].b, m_plane[i].c), vmin) + m_plane[i].d > 0)
				return false;
		}

		return result;
	}
}
