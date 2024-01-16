#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

namespace engine {
	
enum class IntersectionType
{
	
};

class BoundingBox
{
public:
	glm::vec3 m_min;
	glm::vec3 m_max;

public:
	void setIdentity();

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

    glm::vec3 getCenter() const
    {
        return m_min + (m_max - m_min) * 0.5f;
    }

    float getExtend() const
    {
        return glm::length((m_max - m_min)) * 0.5f;
    }

    const glm::vec3 operator() (uint32_t pElem) const
    {
        if (pElem == 0)
            return m_min;
        else if (pElem == 1)
            return m_max;

       // ASSERT(0);
        return glm::vec3(0.f);
    }

    glm::vec3 operator() (uint32_t pElem)
    {
        if (pElem == 0)
            return m_min;
        else if (pElem == 1)
            return m_max;

       // ASSERT(0);
        return glm::vec3(0.f);
    }
};
	
}

#endif