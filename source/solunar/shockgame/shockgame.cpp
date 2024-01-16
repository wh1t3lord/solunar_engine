#include "shockgame/shockgamepch.h"

#include "shockgame/shockgame.h"
#include "shockgame/shockeventlistener.h"
#include "shockgame/shocksignalmanager.h"

#include "shockgame/shockplayercontroller.h"

namespace engine
{

// implemetation for IGameInterface

static ShockGameInterface s_shockGameInterface;
IGameInterface* g_gameInterface = (IGameInterface*)&s_shockGameInterface;

ShockGameInterface* engine::getShockGameInterface()
{
	return &s_shockGameInterface;
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
	// register game objects
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