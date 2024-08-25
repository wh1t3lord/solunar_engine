#include "gamepch.h"
#include "weaponautocomponent.h"

namespace solunar
{

IMPLEMENT_OBJECT(WeaponAutoComponent, WeaponComponent);

void WeaponAutoComponent::RegisterObject()
{
    g_typeManager->RegisterObject<WeaponAutoComponent>();
}

WeaponAutoComponent::WeaponAutoComponent()
{
}

WeaponAutoComponent::~WeaponAutoComponent()
{
}

void WeaponAutoComponent::Update(float dt)
{
    WeaponComponent::Update(dt);
}
    
}
