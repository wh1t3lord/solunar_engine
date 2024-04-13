#include "fpsgamepch.h"
#include "fpsgame.h"

// Base game includes
#include "gameframework/gamelogic/camera/cameramanager.h"
#include "gameframework/gamelogic/mainmenu/mainmenuworldcomponent.h"
#include "gameframework/gamelogic/player/playercontrollercomponent.h"
#include "gameframework/gamelogic/player/weaponchaincomponent.h"
#include "gameframework/gamelogic/saveload/saveloadmanager.h"
#include "gameframework/gamelogic/weapons/weaponcomponent.h"
#include "gameframework/gamelogic/weapons/weaponautocomponent.h"
#include "gameframework/gamelogic/weapons/weaponpistolcomponent.h"
#include "gameframework/gamelogic/world/levelmanager.h"

#include "graphics/imguimanager.h"

namespace engine
{

// implemetation for IGameInterface

static FPSGameInterface s_fpsGameInterface;
__declspec(dllexport) IGameInterface* g_gameInterface = (IGameInterface*)&s_fpsGameInterface; 

FPSGameInterface* getFPSGameInterface()
{
	return &s_fpsGameInterface;
}


void registerGameClasses()
{
	// Register of engine game objects goes here
	
	MainMenuWorldComponent::registerObject();
	PlayerControllerComponent::registerObject();
	WeaponComponent::registerObject();
	WeaponAutoComponent::registerObject();
	WeaponPistolComponent::registerObject();
	WeaponChainComponent::registerObject();
	LevelManagerComponent::registerObject();

	//TestRotatorComponent::registerObject();
}

void registerFPSClasses()
{
	// Custom register goes here 
	Core::msg("Registring FPS classes");
	//ShockSignal::registerObject();
	 
	
}

FPSGameInterface::FPSGameInterface()
{
}

FPSGameInterface::~FPSGameInterface()
{
}

void FPSGameInterface::initialize()
{
	Logger::init();
	Core::msg("FPSGameInterface: Initializing FPS game");

	// register game objects
	registerGameClasses();

	// register сustom objects
	registerFPSClasses();

	// add event listener
//	g_eventManager.addEventListener(&g_shockEventListener);

	// Create signal manager
	//FPSSignalManager::createInstance(); # Alivebyte: idk what it's for now
}

void FPSGameInterface::shutdown()
{
	// Destroy signal manager
	//FPSSignalManager::destroyInstance(); # same as line 77
	
	Core::msg("FPSGameInterface: Shutting down FPS game");
	
	// g_eventManager.removeEventListener(&g_shockEventListener);
}

}