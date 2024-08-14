#ifndef WEAPONCOMPONENT_H
#define WEAPONCOMPONENT_H

#include "engine/entity/logiccomponent.h"

namespace solunar {

	enum class WeaponsType
	{
		None,
		Pistol,
		Shotgun,
	};
	
	class WeaponComponent : public LogicComponent
	{
		IMPLEMENT_OBJECT(WeaponComponent, LogicComponent);
	public:
		WeaponComponent();
		virtual ~WeaponComponent();

		static void RegisterObject();

		virtual void Update(float dt);

	private:
		bool m_inited;
		int m_idleAni;
		int m_fireAni;

		int m_ammo;
	};
}
#endif // ! WEAPONCOMPONENT_H