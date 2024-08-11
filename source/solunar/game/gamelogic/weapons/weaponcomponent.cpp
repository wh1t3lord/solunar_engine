#include "gamepch.h"
#include "weaponcomponent.h"

namespace solunar
{
	WeaponComponent::WeaponComponent()
	{

	}

	WeaponComponent::~WeaponComponent()
	{

	}

	void WeaponComponent::RegisterObject()
	{
		g_typeManager->RegisterObject<WeaponComponent>();
	}

	void WeaponComponent::Update(float dt)
	{

	}

}