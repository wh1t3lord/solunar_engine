#ifndef WEAPONPISTOLCOMPONENT_H
#define WEAPONPISTOLCOMPONENT_H

#include "game/gamelogic/weapons/weaponautocomponent.h"

namespace engine
{

class WeaponPistolComponent : public WeaponAutoComponent
{
    ImplementObject(WeaponPistolComponent, WeaponAutoComponent);
public:
    WeaponPistolComponent();
    ~WeaponPistolComponent();

    static void registerObject();
};
    
}

#endif