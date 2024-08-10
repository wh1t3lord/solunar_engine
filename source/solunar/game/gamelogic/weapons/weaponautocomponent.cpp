#include "gamepch.h"
#include "weaponautocomponent.h"

namespace solunar
{

void WeaponAutoComponent::registerObject()
{
    g_typeManager->registerObject<WeaponAutoComponent>();
}

WeaponAutoComponent::WeaponAutoComponent()
{
}

WeaponAutoComponent::~WeaponAutoComponent()
{
}

void WeaponAutoComponent::update(float dt)
{
    WeaponComponent::update(dt);
}
    
}
