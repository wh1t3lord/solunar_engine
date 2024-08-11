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
		ImplementObject(WeaponComponent, LogicComponent);
	public:
		WeaponComponent();
		virtual ~WeaponComponent();

		static void registerObject();

		virtual void update(float dt);

	private:
		bool m_inited;
		int m_idleAni;
		int m_fireAni;
	};
}
#endif // ! WEAPONCOMPONENT_H