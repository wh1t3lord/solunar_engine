#include "gamepch.h"
#include "weaponcomponent.h"

namespace engine
{
	WeaponComponent::WeaponComponent()
	{

	}

	WeaponComponent::~WeaponComponent()
	{

	}

	void WeaponComponent::registerObject()
	{
		g_typeManager->registerObject<WeaponComponent>();
	}

	void WeaponComponent::update(float dt)
	{

	}

}