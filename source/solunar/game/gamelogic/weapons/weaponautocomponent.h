#ifndef WEAPONAUTOCOMPONENT_H
#define WEAPONAUTOCOMPONENT_H

#include "game/gamelogic/weapons/weaponcomponent.h"

namespace solunar
{

class WeaponAutoComponent : public WeaponComponent
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