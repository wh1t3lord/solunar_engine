#include "gamepch.h"
#include "gameframework/gamelogic/player/weaponchaincomponent.h"

namespace engine
{

void WeaponChainComponent::registerObject()
{
	g_typeManager->registerObject<WeaponChainComponent>();
}

WeaponChainComponent::WeaponChainComponent() :
	m_nextChain(nullptr),
	m_weapon(nullptr)
{
}

WeaponChainComponent::~WeaponChainComponent()
{
	m_nextChain = nullptr;
	m_weapon = nullptr;
}

}