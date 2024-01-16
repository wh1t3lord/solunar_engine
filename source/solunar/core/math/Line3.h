#ifndef     _LINE3_H_
#define     _LINE3_H_
 
#include "core/math/math_types.h"

namespace engine {


/**
 *  Representation of a ray in 3d.
 */
template <class T>
class Ray3
{
public:
    /**
     *  Constructor.
     */
    Ray3(){}

    /**
     *  Construct a line given an origin and a direction.
     *  @param  pOrigin     The origin of the line.
     *  @param  pDirection  The direction of the line.
     */
    Ray3( const glm::vec3& pOrigin, const glm::vec3& pDirection )
        : mOrigin(pOrigin)
        , mDirection(pDirection)
    {
        mInvDirection.x = 1 / pDirection.x;
        mInvDirection.y = 1 / pDirection.y;
        mInvDirection.z = 1 / pDirection.z;
        mSign[0] = mInvDirection.x < 0;
        mSign[1] = mInvDirection.y < 0;
        mSign[2] = mInvDirection.z < 0;
    }
		
    /**
     *  Get the direction of this line.
     *  return The direction of this line.     
     */
    const glm::vec3& GetOrigin() const
    {
        return mOrigin;
    }

    /**
     *  Get the direction of this line.
     *  return The direction of this line.     
     */
	const glm::vec3& GetDirection() const
    {
        return mDirection;
    }

    /**
     *  Get the direction of this line.
     *  return The direction of this line.     
     */
	const glm::vec3& GetInvDirection() const
    {
        return mInvDirection;
    }

    /**
     *  Set the origin of this line.
     *  @param  pOrigin  New origin of this line.
     */
    void SetOrigin( const glm::vec3& pOrigin )
    {
        mOrigin = pOrigin;
    }

    /**
     *  Set the direction of this line.
     *  @param  pDirection  New direction of this line.
     */
	void SetDirection( const glm::vec3& pDirection )
    {
        mDirection = pDirection;

        mInvDirection.x = 1 / pDirection.x;
        mInvDirection.y = 1 / pDirection.y;
        mInvDirection.z = 1 / pDirection.z;
        mSign[0] = mInvDirection.x < 0;
        mSign[1] = mInvDirection.y < 0;
        mSign[2] = mInvDirection.z < 0;
    }

    /**
     *  Return the sign of the inverse direction vector element.
     */
    bool GetSign( uint32_t pElem ) const
    {
        return mSign[pElem];
    }

public:
    glm::vec3 mOrigin;         //!< Direction of this line.
	glm::vec3 mDirection;      //!< Orientation of this line.
    glm::vec3 mInvDirection;   //!< Inverse of direction.
    bool       mSign[3];        //!< Sign of inv. direction.
};


typedef Ray3<float>    Ray3f;
typedef Ray3<double>   Ray3d;


} // namespace Gamedesk


#endif  //  _LINE3_H_
