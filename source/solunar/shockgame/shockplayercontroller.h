#ifndef SHOCKPLAYERCONTROLLER_H
#define SHOCKPLAYERCONTROLLER_H

#include "engine/physics/shapescomponent.h"
#include "engine/physics/rigidbodycomponent.h"
#include "graphics/mesh.h"
#include "game/gamelogic/player/playercontrollercomponent.h"

namespace solunar
{

class CameraFirstPersonComponent;

struct ShockPlayerStats
{
	float m_endurance;
	float m_health;
	float m_mana;
	int m_money;
};

class ShockPlayerController : public PlayerControllerComponent
{
	DECLARE_OBJECT(ShockPlayerController);
	DECLARE_PROPERTY_REGISTER(ShockPlayerController);
public:
	ShockPlayerController();
	~ShockPlayerController();

	static void RegisterObject();

	void OnEntitySet(Entity* entity) override;
	void OnEntityRemove() override;

	void Update(float dt) override;

	void doHit(float amount);

	Entity* GetActiveWeaponEntity() { return m_activeWeaponEntity; }

	const ShockPlayerStats& GetPlayerStats() { return m_playerStats; }

private:
	void ActivateCamera();
	void InitializeCamera();
	void InitializeComponents();
	void UpdateCamera(float dt);
	void UpdateMovement(float dt);
	void UpdateLogic(float dt);
	void DebugUpdate(float dt);

private:
	ShockPlayerStats m_playerStats;

	RigidBodyProxyComponent* m_rigidBody;

	Entity* m_cameraEntity;
	CameraFirstPersonComponent* m_camera;

	Entity* m_weaponEntity;
	AnimatedMeshComponent* m_weaponMesh;

	Entity* m_activeWeaponEntity;

	glm::vec3 m_weaponSwayAngles;

public:
	bool m_flyCam;
};

void shockGamePlayerDebug(bool* open);

}

#endif // !SHOCKPLAYERCONTROLLER_H
