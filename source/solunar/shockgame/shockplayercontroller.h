#ifndef SHOCKPLAYERCONTROLLER_H
#define SHOCKPLAYERCONTROLLER_H

#include "engine/physics/shapescomponent.h"
#include "engine/physics/rigidbodycomponent.h"
#include "graphics/mesh.h"
#include "game/gamelogic/player/playercontrollercomponent.h"

namespace engine
{

struct ShockPlayerStats
{
	float m_endurance;
	float m_health;
	float m_mana;
};

class ShockPlayerController : public PlayerControllerComponent
{
	ImplementObject(ShockPlayerController, PlayerControllerComponent);
public:
	ShockPlayerController();
	~ShockPlayerController();

	static void registerObject();

	void onEntitySet(Entity* entity) override;
	void onEntityRemove() override;

	void update(float dt) override;

	void doHit(float amount);

private:
	void activateCamera();
	void initializeCamera();
	void initializeComponents();
	void updateCamera(float dt);
	void updateMovement(float dt);
	void debugUpdate(float dt);

private:
	ShockPlayerStats m_playerStats;

	RigidBodyProxyComponent* m_rigidBody;

	Entity* m_cameraEntity;
	CameraFirstPersonComponent* m_camera;

	Entity* m_weaponEntity;
	AnimatedMeshComponent* m_weaponMesh;

public:
	bool m_flyCam;
};

}

#endif // !SHOCKPLAYERCONTROLLER_H
