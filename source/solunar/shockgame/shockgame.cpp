#include "shockgame/shockgamepch.h"

#include "shockgame/shockgame.h"
#include "shockgame/shockeventlistener.h"
#include "shockgame/shocksignalmanager.h"

#include "shockgame/shockplayercontroller.h"

// Base game includes
#include "game/gamelogic/camera/cameramanager.h"
#include "game/gamelogic/mainmenu/mainmenuworldcomponent.h"
#include "game/gamelogic/player/playercontrollercomponent.h"
#include "game/gamelogic/player/weaponchaincomponent.h"
#include "game/gamelogic/saveload/saveloadmanager.h"
#include "game/gamelogic/weapons/weaponcomponent.h"
#include "game/gamelogic/weapons/weaponautocomponent.h"
#include "game/gamelogic/weapons/weaponpistolcomponent.h"
#include "game/gamelogic/world/levelmanager.h"

namespace engine
{

// implemetation for IGameInterface

static ShockGameInterface s_shockGameInterface;
IGameInterface* g_gameInterface = (IGameInterface*)&s_shockGameInterface;

ShockGameInterface* getShockGameInterface()
{
	return &s_shockGameInterface;
}

class TestRotatorComponent : public LogicComponent
{
	ImplementObject(TestRotatorComponent, LogicComponent);

public:
	TestRotatorComponent() : m_YAxis(0.0f)
	{}

	~TestRotatorComponent()
	{}

	static void registerObject()
	{
		g_typeManager->registerObject<TestRotatorComponent>();
	}

	void update(float dt) override
	{
		m_YAxis += dt * 200.0f;
		getEntity()->quaternionRotate(glm::vec3(0.f, 1.f, 0.f), m_YAxis);
	}

private:
	float m_YAxis;
};

void registerGameClasses()
{
	MainMenuWorldComponent::registerObject();
	PlayerControllerComponent::registerObject();
	WeaponComponent::registerObject();
	WeaponAutoComponent::registerObject();
	WeaponPistolComponent::registerObject();
	WeaponChainComponent::registerObject();
	LevelManagerComponent::registerObject();

	TestRotatorComponent::registerObject();
}

void registerShockClasses()
{
	ShockSignal::registerObject();
	 
	ShockPlayerController::registerObject();
}

ShockGameInterface::ShockGameInterface()
{
}

ShockGameInterface::~ShockGameInterface()
{
}

void ShockGameInterface::initialize()
{
	Core::msg("Initializing game");

	// register game objects
	registerGameClasses();

	// register shock objects
	registerShockClasses();

	// add event listener
//	g_eventManager.addEventListener(&g_shockEventListener);

	// Create signal manager
	ShockSignalManager::createInstance();
}

void ShockGameInterface::shutdown()
{
	// Destroy signal manager
	ShockSignalManager::destroyInstance();

	// g_eventManager.removeEventListener(&g_shockEventListener);
}

}