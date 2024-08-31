#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

namespace solunar {
	
class BoundingBox
{
public:
	glm::vec3 m_min;
	glm::vec3 m_max;

public:
	void SetIdentity();

    //! Returns a bool whether the given point is inside the bounding volume.
    bool Contains(const glm::vec3& pPoint) const
    {
        return m_min.x <= pPoint.x && m_min.y <= pPoint.y && m_min.z <= pPoint.z &&
            m_max.x >= pPoint.x && m_max.y >= pPoint.y && m_max.z >= pPoint.z;
    }

    bool Contains(const BoundingBox& pBox) const
    {
        return Contains(pBox.m_min) && Contains(pBox.m_max);
    }

    glm::vec3 GetCenter() const
    {
        return m_min + (m_max - m_min) * 0.5f;
    }

    float GetExtend() const
    {
        return glm::length((m_max - m_min)) * 0.5f;
    }
};
	
}

#endif