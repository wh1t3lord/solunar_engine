#ifndef MATH_MATH_TYPES_H
#define MATH_MATH_TYPES_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace engine
{
	namespace maths {

		const float     HALF_PI = 1.5707963267948966192313216916398f;    //!< PI.
		const float     PI = 3.1415926535897932384626433832795f;    //!< PI.
		const float     PI_2 = 6.283185307179586476925286766559f;    //!< 2*PI.
		const float     PI_ON_180 = 0.017453292519943295769236907684886f; //!< PI/180.
		const float     ONE_180_ON_PI = 57.295779513082320876798154814105f;   //!< 180/PI
		const float     EPSILON = 1e-6f;                                //!< A very small float value.
	}
}

#endif // !MATH_MATH_TYPES_H
