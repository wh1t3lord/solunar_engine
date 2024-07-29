#include "shockgamepch.h"
#include "shockgame/shockprojectilecomponent.h"
#include "shockgame/shockplayercontroller.h"

#include "engine/physics/rigidbodycomponent.h"

#include "graphics/debugrenderer.h"

namespace engine
{

BeginPropertyRegister(ShockProjectileComponent)
{
	RegisterProperty(ShockProjectileComponent, PropertyVector3, m_direction);
	RegisterProperty(ShockProjectileComponent, PropertyFloat, m_speed);
}
EndPropertyRegister(ShockProjectileComponent)

ShockProjectileComponent::ShockProjectileComponent() :
	m_direction(0.0f),
	m_speed(0.0f)
{
}

ShockProjectileComponent::~ShockProjectileComponent()
{
}

void ShockProjectileComponent::onSpawn(const glm::vec3& velocity, const glm::vec3& direction,
	const std::string& visualFilename)
{
#if 0
	// create rigid body
	BoxShapeComponent* boxShape = getEntity()->createComponent<BoxShapeComponent>();
	boxShape->createShape(glm::vec3(0.1f));

	RigidBodyComponent* rigidBody = getEntity()->createComponent<RigidBodyComponent>();
	rigidBody->attachShape(boxShape);
	rigidBody->setMass(0.02f);
	rigidBody->setLinearVelocity(velocity);
#endif

	m_direction = velocity;
}

void ShockProjectileComponent::onEntitySet(Entity* entity)
{
	Component::onEntitySet(entity);

	Assert2(!entity->getComponent<RigidBodyComponent>(), "Projectile cannot' have already attached rigid body!");
}

void ShockProjectileComponent::update(float dt)
{
	g_debugRender.drawBoundingBox(getEntity()->getBoundingBox(), glm::vec3(1.0f, 0.0, 0.0f));

	const float kSpeed = 1.2f;

	glm::vec3 position = getEntity()->getPosition();
	position = glm::lerp(position, m_direction, kSpeed * dt);
	getEntity()->setPosition(position);

	// find player
	std::vector<Entity*> players = getWorld()->getEntityManager().getEntitiesWithComponent<ShockPlayerController>();
	Entity* player = players[0];

	if (getEntity()->getBoundingBox().contains(player->getPosition())) // we shot!
	{
		//ShockPlayerController* playerController = player->getComponent<ShockPlayerController>();
		//playerController->doHit(25.0f);
	}
}

/////////////////////////////////////////////////////////////////////
// Projectile Pool

ShockProjectilePool ShockProjectilePool::ms_instance;

ShockProjectilePool::ShockProjectilePool()
{
	for (int i = 0; i < m_pool.size(); i++)
		m_pool[i] = nullptr;
}

ShockProjectilePool::~ShockProjectilePool()
{
}

Entity* ShockProjectilePool::createProjectile(ShockProjectileType projectileType, const glm::vec3& position, const glm::vec3& direction, const glm::vec3& velocity)
{
	Assert2(m_poolCount > 256, "Reached maximum of the pool");

	Entity* entity = m_pool[m_poolCount++];
	entity->setPosition(position);

	return entity;
}

}