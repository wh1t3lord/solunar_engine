#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

namespace engine {
	
class BoundingBox
{
public:
	glm::vec3 m_min;
	glm::vec3 m_max;

public:
	void setIdentity();

    BoundingBox& transformAABB(const glm::mat4& modelMatrix);

    //! Returns a bool whether the given point is inside the bounding volume.
    bool contains(const glm::vec3& pPoint) const
    {
        return m_min.x <= pPoint.x && m_min.y <= pPoint.y && m_min.z <= pPoint.z &&
            m_max.x >= pPoint.x && m_max.y >= pPoint.y && m_max.z >= pPoint.z;
    }

    bool contains(const BoundingBox& pBox) const
    {
        return contains(pBox.m_min) && contains(pBox.m_max);
    }

    glm::vec3 getCenter() const
    {
        return m_min + (m_max - m_min) * 0.5f;
    }

    float getExtend() const
    {
        return glm::length((m_max - m_min)) * 0.5f;
    }
};
	
}

#endif