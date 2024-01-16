#include "gamepch.h"
#include "weaponpistolcomponent.h"

namespace engine
{
    
WeaponPistolComponent::WeaponPistolComponent()
{
}

WeaponPistolComponent::~WeaponPistolComponent()
{
}

void WeaponPistolComponent::registerObject()
{
    g_typeManager->registerObject<WeaponPistolComponent>();
}
    
}
