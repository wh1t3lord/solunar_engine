#ifndef SHOCKPROJECTILECOMPONENT_H
#define SHOCKPROJECTILECOMPONENT_H

#include <array>

namespace engine
{

enum ShockProjectileType
{
	ShockProjectileType_Default,
	ShockProjectileType_Camera,
	ShockProjectileType_Pistol,
	ShockProjectileType_AssaultRifle,
	ShockProjectileType_Shotgun,
	ShockProjectileType_Grenade,
};

class ShockProjectileComponent : public LogicComponent
{
	ImplementObject(ShockProjectileComponent, LogicComponent);
	DeclarePropertyRegister(ShockProjectileComponent);
public:
	ShockProjectileComponent();
	~ShockProjectileComponent();

	// #TODO: Make it key-values based and method of base class!
	void onSpawn(const glm::vec3& velocity, const glm::vec3& direction,
		const std::string& visualFilename);

	void onEntitySet(Entity* entity) override;

	void update(float dt) override;

private:
	glm::vec3 m_direction;
	float m_speed;
};

/////////////////////////////////////////////////////////////////////
// Projectile Pool

class ShockProjectilePool : public Singleton<ShockProjectilePool>
{
	static ShockProjectilePool ms_instance;
public:
	ShockProjectilePool();
	~ShockProjectilePool();

	/**
		/brief Return any free Entity from pool.
		/param projectileType	Specific projectile type (ShockProjectileType_Default have very small velocity value)
		/param position			Position of projectile starting point
		/param direction		Normalized direction for projectile
		/param velocity			Velocity of projectile(will be ignored by any projectile execpt ShockProjectileType_Default and ShockProjectileType_Grenade)
	*/
	Entity* createProjectile(ShockProjectileType projectileType,
		const glm::vec3& position,
		const glm::vec3& direction,
		const glm::vec3& velocity = glm::vec3(1.0f));

private:
	std::array<Entity*, 256> m_pool;
	int m_poolCount;
};

}

#endif // !SHOCKPROJECTILECOMPONENT_H
