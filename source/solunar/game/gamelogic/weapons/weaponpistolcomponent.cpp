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

void WeaponPistolComponent::RegisterObject()
{
    g_typeManager->RegisterObject<WeaponPistolComponent>();
}
    
}
