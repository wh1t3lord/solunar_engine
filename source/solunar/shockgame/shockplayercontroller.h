#ifndef SHOCKPLAYERCONTROLLER_H
#define SHOCKPLAYERCONTROLLER_H

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

	// std::shared_ptr<TransformComponent> m_transform;
	// std::shared_ptr<RigidBodyComponent> m_rigidBody;
	// 
	// std::shared_ptr<CameraFirstPersonComponent> m_camera;
	// std::shared_ptr<TransformComponent> m_cameraTransform;
};

}

#endif // !SHOCKPLAYERCONTROLLER_H
