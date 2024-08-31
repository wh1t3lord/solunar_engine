#ifndef WEAPONAUTOCOMPONENT_H
#define WEAPONAUTOCOMPONENT_H

#include "game/gamelogic/weapons/weaponcomponent.h"

namespace solunar
{

class WeaponAutoComponent : public WeaponComponent
{
    DECLARE_OBJECT(WeaponAutoComponent);
public:
    static void RegisterObject();

    WeaponAutoComponent();
    ~WeaponAutoComponent();

    virtual void Update(float dt);
};
    
}

#endif