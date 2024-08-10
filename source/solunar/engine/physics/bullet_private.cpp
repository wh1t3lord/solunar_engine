#include "enginepch.h"
#include "physics/bullet_private.h"

namespace solunar {

glm::vec3 btVectorToGlm(const btVector3& vec)
{
	return glm::vec3(vec.x(), vec.y(), vec.z());
}

btVector3 glmVectorToBt(const glm::vec3& vec)
{
	return btVector3(vec.x, vec.y, vec.z);
}

}