#include "core/math/math_types.h"
#include "core/math/boundingbox.h"

namespace engine {

	void BoundingBox::setIdentity()
	{
		m_min = glm::vec3(-1.0f);
		m_max = glm::vec3(1.0f);
	}

}