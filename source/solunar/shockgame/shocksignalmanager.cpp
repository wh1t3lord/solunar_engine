#include "shockgamepch.h"
#include "shockgame/shocksignalmanager.h"

namespace solunar
{

ShockSignalManager* ShockSignalManager::ms_instance = nullptr;

void ShockSignalManager::createInstance()
{
	Assert2(!ms_instance, "Canno't create signal manager twice");
	ms_instance = mem_new<ShockSignalManager>();
}

void ShockSignalManager::destroyInstance()
{
	if (ms_instance)
	{
		mem_delete(ms_instance);
		ms_instance = nullptr;
	}
}

ShockSignalManager* ShockSignalManager::getInstance()
{
	return Singleton<ShockSignalManager>::getInstance();
}

ShockSignalManager::ShockSignalManager()
{
}

ShockSignalManager::~ShockSignalManager()
{
}

//////////////////////////////
// Shock signal implementation
//////////////////////////////

ShockSignal::ShockSignal()
{
}

ShockSignal::~ShockSignal()
{
}

void ShockSignal::registerObject()
{
	g_typeManager->registerObject<ShockSignal>();
}

}