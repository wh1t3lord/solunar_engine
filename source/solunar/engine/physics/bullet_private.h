#ifndef BULLET_PRIVATE_H
#define BULLET_PRIVATE_H

// Bullet includes
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"

namespace solunar {

const int kCollisionFilterAllMask = btBroadphaseProxy::DefaultFilter | btBroadphaseProxy::StaticFilter | btBroadphaseProxy::KinematicFilter | btBroadphaseProxy::DebrisFilter;

glm::vec3 btVectorToGlm(const btVector3& vec);
btVector3 glmVectorToBt(const glm::vec3& vec);


}

#endif