#ifndef     _PLANE3_H_
#define     _PLANE3_H_

#include "core/core.h"
#include "core/platform/core_platform.h"
#include "core/utils/common.h"
#include "core/debug.h"
#include "core/math/math_types.h"
#include "core/math/Line3.h"

namespace engine {

/**
 *  Representation of an infinite plane in 3D space.
 *  This class keeps the plane information as a plane formula (A,B,C,D),
 *  where ABC is the normal and D the constant. 
 */
template <class T> 
class Plane3
{
public:
    //! Side of the plane.
    enum Side
    {
        NoSide,            //!< The plane itself.
        PositiveSide,      //!< Side of the plane where the normal is facing.
        NegativeSide       //!< Side of the plane where the normal is facing away.   
    };

    //! Constructor.
    Plane3() {}

    //! Copy constructor.
    Plane3( const Plane3& pOther )  { memcpy( mABCD, pOther.mABCD, 4*sizeof(T) ); }

    /**
     *  Construct a plane using the plane normal and the constant.
     *  @param  pNormal     The plane normal.
     *  @param  pConstant   The plane constant.
     */
    Plane3( const glm::vec3& pNormal, T pConstant )
    {
        mABCD[0] = pNormal.x;
        mABCD[1] = pNormal.y;
        mABCD[2] = pNormal.z;
        mABCD[3] = pConstant;
    }

    /**
     *  Construct a plane using the plane normal and a point lying on the plane.
     *  @param  pNormal The plane normal.
     *  @param  pPoint  A point on the plane.
     */
    Plane3( const glm::vec3& pNormal, const glm::vec3& pPoint )
    {
        mABCD[0] = pNormal.x;
        mABCD[1] = pNormal.y;
        mABCD[2] = pNormal.z;
        mABCD[3] = glm::dot( pNormal, pPoint);
    }

    /**
     *  Construct a plane using 3 unit length vectors where 
     *  (pPoint2 - pPoint1) cross (pPoint3 - pPoint1) is the plane normal and
     *  normal dot pPoint1 is the constant.
     *  @param  pPoint1     First vector (must be unit length).
     *  @param  pPoint2     Second vector (must be unit length).
     *  @param  pPoint3     Third vector (must be unit length).
     */
    Plane3( const glm::vec3& pPoint1, const glm::vec3& pPoint2, const glm::vec3& pPoint3 )
    {
        glm::vec3 normal = glm::cross( (pPoint2 - pPoint1) , (pPoint3 - pPoint1));
        mABCD[0] = normal.x;
        mABCD[1] = normal.y;
        mABCD[2] = normal.z;
        mABCD[3] = glm::dot(normal, pPoint1);
    }
   
    /**
     *	Set the plane normal.
     *  @parame pNormal The new plane normal.
     */
    void SetNormal( const glm::vec3& pNormal )
    {
        mABCD[0] = pNormal.x; mABCD[1] = pNormal.y; mABCD[2] = pNormal.z;
    }

    /**
     *	Get the plane normal.
     *  @return The plane normal.
     */
    glm::vec3 GetNormal() const
    {
        return glm::vec3( mABCD[0], mABCD[1], mABCD[2] );
    }

    /**
     *	Set the plane constant.
     *  @param  pConstant   The plane constant.
     */
    void SetConstant( T pConstant ) { mABCD[3] = pConstant; }

    /**
     *	Get the plane constant.
     *  @return The plane constant.
     */
    T    GetConstant() const        { return mABCD[3]; }
    
    /**
     *  Get a pointer to the internal representation of the plane.
     *  @brief  Indirection operator (const).
     *  @return A pointer to a T array.
     */
    operator const T* () const
    {
        return mABCD;
    }
    
    /**
     *  Get a pointer to the internal representation of the plane.
     *  @brief  Indirection operator.
     *  @return A pointer to a T array.
     */
    operator T* ()
    {
        return mABCD;
    }

    /**
     *  Access the plane equation as an array of T.
     *	@brief  Subscript operator (const).
     *  @param  pIndex  Index of the element (between [0,3]).
     *  @return The element of the equation at position pIndex.
     */
    const T& operator () ( uint32_t pIndex ) const
    {
        Assert( pIndex < 4 );
        return mABCD[pIndex];
    }

    /**
     *  Access the plane equation as an array of T.
     *	@brief  Subscript operator.
     *  @param  pIndex  Index of the element (between [0,3]).
     *  @return The element of the equation at position pIndex.
     */
    T& operator () (uint32_t pIndex )
    {
        Assert( pIndex < 4 );
        return mABCD[pIndex];
    }

    /**
     *  Assignment operator.	
     *  @param  pOther  Value to be assigned to this plane.
     *  @return A reference to this plance.
     */
    const Plane3& operator = ( const Plane3& pOther )
    {
        memcpy( mABCD, pOther.mABCD, 4*sizeof(T) );
        return *this;
    }

    /**
     *  Return the side where a given point lies in relation to this plane.
     *  @param  pPoint  The point used for the test.
     *  @return The side where the point is located.
     */
    Side WhichSide( const glm::vec3& pPoint ) const
    {
        T distance = DistanceTo(pPoint);

        if ( distance < (T)0.0 )
            return Plane3<T>::NegativeSide;

        if ( distance > (T)0.0 )
            return Plane3<T>::PositiveSide;

        return Plane3<T>::NoSide;
    }

    /**
     *  Obtain the distance between a point and this plane.
     *  It's in fact a peusdodistance, as it will be negative if the point is on
     *  the negative side of the plane.  The absolute value of the return value
     *  is the true distance only when the plane normal is a unit length vector.
     *  @param  pPoint  The point used for the test.
     *  @return The pseudodistance between the point and this plane.
     */
    T DistanceTo( const glm::vec3& pPoint ) const
    {
        return mABCD[0]*pPoint.x + mABCD[1]*pPoint.y + mABCD[2]*pPoint.z - mABCD[3];
    }

    /**
     *  Get a normalized version of this plane.
     *  @return A new normalized plane.
     */
    Plane3 GetNormalized() const
    {
        T sqrLength = mABCD[0]*mABCD[0] + mABCD[1]*mABCD[1] + mABCD[2]*mABCD[2];
        T invLength = 1.0 / sqrt(sqrLength);

        return Plane3( glm::vec3(mABCD[0]*invLength, mABCD[1]*invLength, mABCD[2]*invLength), mABCD[3]*invLength );
    }

    /**
     *  Normalize this plane.
     */
    void Normalize()
    {
        T sqrLength = mABCD[0]*mABCD[0] + mABCD[1]*mABCD[1] + mABCD[2]*mABCD[2];
        T invLength = 1.0 / sqrt(sqrLength);

        mABCD[0] *= invLength;
        mABCD[1] *= invLength;
        mABCD[2] *= invLength;
        mABCD[3] *= invLength;
    }

protected:
    T   mABCD[4];       //!< The equation of this plane as an array: [0]=A,[1]=B,[2]=C,[3]=D
};


typedef Plane3<float>   Plane3f;
typedef Plane3<double>  Plane3d;


} // namespace Gamedesk


#endif  //  _PLANE3_H_
