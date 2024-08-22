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
		DECLARE_OBJECT(WeaponComponent);
	public:
		WeaponComponent();
		virtual ~WeaponComponent();

		static void RegisterObject();

		virtual void Update(float dt);
	};
}
#endif // ! WEAPONCOMPONENT_H