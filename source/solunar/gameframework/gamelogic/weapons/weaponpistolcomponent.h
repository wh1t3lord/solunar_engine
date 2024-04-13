#ifndef WEAPONPISTOLCOMPONENT_H
#define WEAPONPISTOLCOMPONENT_H

#include "gameframework/gamelogic/weapons/weaponautocomponent.h"

namespace engine
{

class GAME_API WeaponPistolComponent : public WeaponAutoComponent
{
    ImplementObject(WeaponPistolComponent, WeaponAutoComponent);
public:
    WeaponPistolComponent();
    ~WeaponPistolComponent();

    static void registerObject();
};
    
}

#endif