#include "gamepch.h"
#include "game/gamelogic/player/weaponchaincomponent.h"

namespace solunar
{

void WeaponChainComponent::RegisterObject()
{
	g_typeManager->RegisterObject<WeaponChainComponent>();
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