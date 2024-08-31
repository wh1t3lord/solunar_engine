#ifndef VIEW_H
#define VIEW_H

namespace solunar
{
	class View
	{
	public:
		glm::mat4 m_projection, m_view;
		float m_fov, m_znear, m_zfar;
		int m_width, m_height;
		
	public:
		void updateInternalValues();
	
		int getWidth() { return m_width; }
		int getHeight() { return m_height; }
	};
}

#endif // !VIEW_H
