#ifndef WEAPONAUTOCOMPONENT_H
#define WEAPONAUTOCOMPONENT_H

#include "game/gamelogic/weapons/weaponcomponent.h"

namespace solunar
{

class WeaponAutoComponent : public WeaponComponent
{
    IMPLEMENT_OBJECT(WeaponAutoComponent, WeaponComponent);
public:
    static void RegisterObject();

    WeaponAutoComponent();
    ~WeaponAutoComponent();

    virtual void Update(float dt);
};
    
}

#endif