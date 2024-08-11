#include "gamepch.h"
#include "weaponpistolcomponent.h"

namespace solunar
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
