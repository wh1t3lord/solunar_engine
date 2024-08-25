#include "gamepch.h"
#include "weaponpistolcomponent.h"

namespace solunar
{

IMPLEMENT_OBJECT(WeaponPistolComponent, WeaponAutoComponent);

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
