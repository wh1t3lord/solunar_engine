#ifndef WEAPONCOMPONENT_H
#define WEAPONCOMPONENT_H

#include "engine/entity/logiccomponent.h"

namespace solunar {

	enum class WeaponsType
	{
		None,
		Pistol,
		MP5
	};
	
	class WeaponComponent : public LogicComponent
	{
		ImplementObject(WeaponComponent, LogicComponent);
	public:
		WeaponComponent();
		virtual ~WeaponComponent();

		static void registerObject();

		virtual void update(float dt);
	};
}
#endif // ! WEAPONCOMPONENT_H