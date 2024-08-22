#include "shockgamepch.h"
#include "shockgame/shockprojectilecomponent.h"
#include "shockgame/shockplayercontroller.h"

#include "engine/physics/rigidbodycomponent.h"

#include "graphics/debugrenderer.h"

namespace solunar
{

IMPLEMENT_OBJECT(ShockProjectileComponent, LogicComponent);

BEGIN_PROPERTY_REGISTER(ShockProjectileComponent)
{
	REGISTER_PROPERTY(ShockProjectileComponent, PropertyVector3, m_direction);
	REGISTER_PROPERTY(ShockProjectileComponent, PropertyFloat, m_speed);
}
END_PROPERTY_REGISTER(ShockProjectileComponent)

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
	BoxShapeComponent* boxShape = GetEntity()->CreateComponent<BoxShapeComponent>();
	boxShape->createShape(glm::vec3(0.1f));

	RigidBodyComponent* rigidBody = GetEntity()->CreateComponent<RigidBodyComponent>();
	rigidBody->AttachShape(boxShape);
	rigidBody->SetMass(0.02f);
	rigidBody->SetLinearVelocity(velocity);
#endif

	m_direction = velocity;
}

void ShockProjectileComponent::OnEntitySet(Entity* entity)
{
	Component::OnEntitySet(entity);

	Assert2(!entity->GetComponent<RigidBodyComponent>(), "Projectile cannot' have already attached rigid body!");
}

void ShockProjectileComponent::Update(float dt)
{
	g_debugRender.drawBoundingBox(GetEntity()->GetBoundingBox(), glm::vec3(1.0f, 0.0, 0.0f));

	const float kSpeed = 1.2f;

	glm::vec3 position = GetEntity()->GetPosition();
	position = glm::lerp(position, m_direction, kSpeed * dt);
	GetEntity()->SetPosition(position);

	// find player
	std::vector<Entity*> players = GetWorld()->GetEntityManager().GetEntitiesWithComponent<ShockPlayerController>();
	Entity* player = players[0];

	if (GetEntity()->GetBoundingBox().Contains(player->GetPosition())) // we shot!
	{
		//ShockPlayerController* playerController = player->GetComponent<ShockPlayerController>();
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
	entity->SetPosition(position);

	return entity;
}

}