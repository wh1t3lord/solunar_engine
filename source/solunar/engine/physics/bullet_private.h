#ifndef BULLET_PRIVATE_H
#define BULLET_PRIVATE_H

// Bullet includes
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"

namespace solunar {

glm::vec3 btVectorToGlm(const btVector3& vec);
btVector3 glmVectorToBt(const glm::vec3& vec);

}

#endif