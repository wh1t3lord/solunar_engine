#ifndef SHOCKPLAYERCONTROLLER_H
#define SHOCKPLAYERCONTROLLER_H

#include "engine/physics/shapescomponent.h"
#include "engine/physics/rigidbodycomponent.h"
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

private:
	void activateCamera();
	void initializeCamera();
	void initializeComponents();
	void updateCamera(float dt);
	void updateMovement(float dt);

private:
	ShockPlayerStats m_playerStats;
	CameraFirstPersonComponent* m_camera;

	RigidBodyComponent* m_rigidBody;
	BoxShapeComponent* m_shape;


public:
	bool m_flyCam;
};

}

#endif // !SHOCKPLAYERCONTROLLER_H
