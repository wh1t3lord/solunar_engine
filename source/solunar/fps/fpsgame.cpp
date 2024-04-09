#include "fpsgamepch.h"
#include "fpsgame.h"

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
	Core::msg("Initializing fps game");

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
	
	Core::msg("Shutting down fps game");
	
	// g_eventManager.removeEventListener(&g_shockEventListener);
}

}