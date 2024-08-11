#include "core/math/math_types.h"
#include "core/math/boundingbox.h"

namespace solunar {

void BoundingBox::setIdentity()
{
	m_min = glm::vec3(-1.0f);
	m_max = glm::vec3(1.0f);
}

BoundingBox& BoundingBox::transformAABB(const glm::mat4& modelMatrix)
{
	// transform to center/extents box representation
	glm::vec3 center = (m_max + m_min) * glm::vec3(0.5);
	glm::vec3 extents = m_max - center;

	// transform center
	glm::vec3 t_center = glm::vec3(modelMatrix * glm::vec4(center, 1.0));

	// transform extents (take maximum)
	glm::mat3 abs_mat = glm::mat3(abs(glm::vec3(modelMatrix[0])), abs(glm::vec3(modelMatrix[1])), abs(glm::vec3(modelMatrix[2])));
	glm::vec3 t_extents = abs_mat * extents;

	// transform to min/max box representation
	glm::vec3 tmin = t_center - t_extents;
	glm::vec3 tmax = t_center + t_extents;

	m_min = tmin;
	m_max = tmax;

	return *this;
}

}