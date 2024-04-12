#ifndef WEAPONCHAINCOMPONENT_H
#define WEAPONCHAINCOMPONENT_H

#include "gameframework/gamelogic/weapons/weaponcomponent.h"

namespace engine
{

// Компонент цепи оружия
class WeaponChainComponent : public Component
{
	ImplementObject(WeaponChainComponent, Component);
public:
	static void registerObject();
	
	WeaponChainComponent();
	~WeaponChainComponent();

	std::shared_ptr<WeaponComponent> getWeapon() { return m_weapon; }

private:
	std::shared_ptr<WeaponComponent> m_weapon;
	std::shared_ptr<WeaponChainComponent> m_nextChain;
};
	
}

#endif