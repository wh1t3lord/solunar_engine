#include "core/math/math_types.h"
#include "Frustum.h"


namespace engine {
	
	
Frustum::Frustum()
{
}

void Frustum::CalculateFrustum( const glm::mat4& pProjectionMatrix, 
							    const glm::mat4& pModelViewMatrix )
{    
	// Now that we have our modelview and projection matrix, if we combine these 2 matrices,
	// it will give us our clipping planes.  To combine 2 matrices, we multiply them.
    glm::mat4 m(pModelViewMatrix * pProjectionMatrix);

	// Now we actually want to get the sides of the frustum.  To do this we take
	// the clipping planes we received above and extract the sides from them.
#if 0
    mPlanes[Left]   = Plane3f( glm::vec3(-m[ 3] - m[ 0], -m[ 7] - m[ 4], -m[11] - m[ 8]),  m[15] + m[12]);
	mPlanes[Right]  = Plane3f( glm::vec3( m[ 0] - m[ 3],  m[ 4] - m[ 7],  m[ 8] - m[11]), -m[12] + m[15]);
	mPlanes[Bottom] = Plane3f( glm::vec3(-m[ 3] - m[ 1], -m[ 7] - m[ 5], -m[11] - m[ 9]),  m[15] + m[13]);
	mPlanes[Top]    = Plane3f( glm::vec3( m[ 1] - m[ 3],  m[ 5] - m[ 7],  m[ 9] - m[11]), -m[13] + m[15]);
	mPlanes[Front]  = Plane3f( glm::vec3(-m[ 3] - m[ 2], -m[ 7] - m[ 6], -m[11] - m[10]),  m[15] + m[14]);
    mPlanes[Back]   = Plane3f( glm::vec3( m[ 2] - m[ 3],  m[ 6] - m[ 7],  m[10] - m[11]), -m[14] + m[15]);
#endif

    // Normalize the planes.
    mPlanes[Right].Normalize();
    mPlanes[Left].Normalize();
    mPlanes[Top].Normalize();
    mPlanes[Bottom].Normalize();
    mPlanes[Back].Normalize();
    mPlanes[Front].Normalize();
}

float Frustum::SphereDistanceInFrustum( const glm::vec3& pPos, float pRadius )
{
    float d = 0;

   for( uint32_t i = 0; i < NumSides; i++ )
   {
      d = mPlanes[i].DistanceTo( pPos );
      if( d > pRadius )
         return 0;
   }

   return d + pRadius;
}

bool Frustum::PolygonInFrustum( uint32_t pNumPoints, glm::vec3* pPointList )
{
   for( uint32_t iPoint = 0; iPoint < pNumPoints; iPoint++ )
   {
      if( PointInFrustum( pPointList[iPoint] ) )
          return true;
   }

   return false;
}


} // namespace Gamedesk
