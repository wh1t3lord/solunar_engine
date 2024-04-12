#ifndef WEAPONAUTOCOMPONENT_H
#define WEAPONAUTOCOMPONENT_H

#include "game/gamelogic/weapons/weaponcomponent.h"

namespace engine
{

class GAME_API WeaponAutoComponent : public WeaponComponent
{
    ImplementObject(WeaponAutoComponent, WeaponComponent);
public:
    static void registerObject();

    WeaponAutoComponent();
    ~WeaponAutoComponent();

    virtual void update(float dt);
};
    
}

#endif