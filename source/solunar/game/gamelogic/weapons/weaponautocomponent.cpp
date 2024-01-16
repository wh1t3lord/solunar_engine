#include "gamepch.h"
#include "weaponautocomponent.h"

namespace engine
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
