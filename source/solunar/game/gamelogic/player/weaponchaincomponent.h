#ifndef WEAPONCHAINCOMPONENT_H
#define WEAPONCHAINCOMPONENT_H

#include "game/gamelogic/weapons/weaponcomponent.h"

namespace solunar
{

// Компонент цепи оружия
class WeaponChainComponent : public Component
{
	DECLARE_OBJECT(WeaponChainComponent);
public:
	static void RegisterObject();
	
	WeaponChainComponent();
	~WeaponChainComponent();

	std::shared_ptr<WeaponComponent> getWeapon() { return m_weapon; }

private:
	std::shared_ptr<WeaponComponent> m_weapon;
	std::shared_ptr<WeaponChainComponent> m_nextChain;
};
	
}

#endif