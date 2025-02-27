#include "graphicspch.h"
#include "graphics/view.h"

namespace solunar
{
	void View::updateInternalValues()
	{
		m_projection = glm::perspective(glm::radians(m_fov),
			(float)m_width / (float)m_height,
			m_znear,
			m_zfar);
	}
}