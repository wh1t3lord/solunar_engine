#include "shockgamepch.h"
#include "shockgame/shocksignalmanager.h"

namespace solunar
{

ShockSignalManager* ShockSignalManager::ms_instance = nullptr;

void ShockSignalManager::CreateInstance()
{
	Assert2(!ms_instance, "Canno't create signal manager twice");
	ms_instance = mem_new<ShockSignalManager>();
}

void ShockSignalManager::DestroyInstance()
{
	if (ms_instance)
	{
		mem_delete(ms_instance);
		ms_instance = nullptr;
	}
}

ShockSignalManager* ShockSignalManager::GetInstance()
{
	return Singleton<ShockSignalManager>::GetInstance();
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

void ShockSignal::RegisterObject()
{
	g_typeManager->RegisterObject<ShockSignal>();
}

}