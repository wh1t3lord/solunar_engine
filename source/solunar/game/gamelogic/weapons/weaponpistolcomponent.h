#ifndef WEAPONPISTOLCOMPONENT_H
#define WEAPONPISTOLCOMPONENT_H

#include "game/gamelogic/weapons/weaponautocomponent.h"

namespace solunar
{

class WeaponPistolComponent : public WeaponAutoComponent
{
    DECLARE_OBJECT(WeaponPistolComponent);
public:
    WeaponPistolComponent();
    ~WeaponPistolComponent();

    static void RegisterObject();
};
    
}

#endif