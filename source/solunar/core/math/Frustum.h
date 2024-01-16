#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include "core/math/Plane3.h"
#include "core/math/boundingbox.h"

namespace engine {


class Frustum  
{
public:
    enum FrustumSide
    {
	    Right	 = 0,		// The RIGHT side of the frustum
	    Left	 = 1,		// The LEFT	 side of the frustum
	    Bottom	 = 2,		// The BOTTOM side of the frustum
	    Top		 = 3,		// The TOP side of the frustum
	    Back	 = 4,		// The BACK	side of the frustum
	    Front	 = 5,		// The FRONT side of the frustum
        NumSides = 6        // Number of sides...
    };  

	//! Default constructor.
	Frustum();
	
    //! Call this to calculte the frustum.
	void CalculateFrustum( const glm::mat4& pProjectionMatrix, 
					       const glm::mat4& pModelViewMatrix );

    //! Return 0 if the sphere is outside the frustum or return the distance + radius 
	float SphereDistanceInFrustum( const glm::vec3& pPos, float pRadius );

	//! Test if the polygon is inside the frustum.
	bool PolygonInFrustum( uint32_t pNumPoints, glm::vec3* pointlist );

public:
    //! This takes a 3D point and returns true if it's inside of the frustum
    bool PointInFrustum( const glm::vec3& pPos ) const
    {
        return mPlanes[ Right].DistanceTo(pPos) >= 0 &&
               mPlanes[  Left].DistanceTo(pPos) >= 0 &&
               mPlanes[   Top].DistanceTo(pPos) >= 0 &&
               mPlanes[Bottom].DistanceTo(pPos) >= 0 &&
               mPlanes[  Back].DistanceTo(pPos) >= 0 &&
               mPlanes[ Front].DistanceTo(pPos) >= 0;
    }

    //! This takes the center and half the length of the cube.
    bool CubeInFrustum( const glm::vec3& pPos, float pSize ) const
    {
        // Do a quick test first: if sphere is in frustum, then the cube certainly is.
        if( SphereInFrustum(pPos, pSize) )
            return true;
        
        return PointInFrustum( glm::vec3(pPos.x-pSize, pPos.y-pSize, pPos.z-pSize) ) ||
               PointInFrustum( glm::vec3(pPos.x+pSize, pPos.y-pSize, pPos.z-pSize) ) ||
               PointInFrustum( glm::vec3(pPos.x-pSize, pPos.y+pSize, pPos.z-pSize) ) ||
               PointInFrustum( glm::vec3(pPos.x+pSize, pPos.y+pSize, pPos.z-pSize) ) ||
               PointInFrustum( glm::vec3(pPos.x-pSize, pPos.y-pSize, pPos.z+pSize) ) ||
               PointInFrustum( glm::vec3(pPos.x+pSize, pPos.y-pSize, pPos.z+pSize) ) ||
               PointInFrustum( glm::vec3(pPos.x-pSize, pPos.y+pSize, pPos.z+pSize) ) ||
               PointInFrustum( glm::vec3(pPos.x+pSize, pPos.y+pSize, pPos.z+pSize) );
    }

    //! This takes a 3D point and a radius and returns true if the sphere is inside of the frustum
    bool SphereInFrustum( const glm::vec3& pPos, float pRadius ) const
    {
	    for( uint32_t i = 0; i < NumSides; i++ )	
	    {
		    // If the center of the sphere is farther away from the plane than the radius
		    if( mPlanes[i].DistanceTo( pPos ) > pRadius )
	            return false;
	    }
    	
	    // The sphere was inside of the frustum!
	    return true;
    }

    bool BoxInFrustum( const BoundingBox& pBox ) const
    {
        if( SphereInFrustum( pBox.getCenter(), pBox.getExtend() ) )
            return true;

        return PointInFrustum( pBox.m_min ) || 
               PointInFrustum( pBox.m_max ) ||
               PointInFrustum( glm::vec3(pBox.m_min.x, pBox.m_min.y, pBox.m_max.z) ) || 
               PointInFrustum( glm::vec3(pBox.m_max.x, pBox.m_min.y, pBox.m_min.z) ) || 
               PointInFrustum( glm::vec3(pBox.m_max.x, pBox.m_min.y, pBox.m_max.z) ) || 
               PointInFrustum( glm::vec3(pBox.m_min.x, pBox.m_max.y, pBox.m_min.z) ) || 
               PointInFrustum( glm::vec3(pBox.m_min.x, pBox.m_max.y, pBox.m_max.z) ) || 
               PointInFrustum( glm::vec3(pBox.m_max.x, pBox.m_max.y, pBox.m_min.z) );      
    }

private:
	// This holds the A B C and D values for each side of our frustum.
	Plane3f     mPlanes[6];
};


} // namespace Gamedesk


#endif // _FRUSTUM_H_
