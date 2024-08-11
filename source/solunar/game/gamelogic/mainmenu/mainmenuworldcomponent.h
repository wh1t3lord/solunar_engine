#ifndef MAINMENU_WORLDCOMPONENT_H
#define MAINMENU_WORLDCOMPONENT_H

#include "engine/entity/logiccomponent.h"

namespace solunar {
	
	class MainMenuWorldComponent : public LogicComponent
	{
		IMPLEMENT_OBJECT(MainMenuWorldComponent, LogicComponent);
	public:
		static void RegisterObject();
		
	public:
		MainMenuWorldComponent();
		~MainMenuWorldComponent();
		
		// Component inheritance
		void OnWorldSet(World* world);
	};
	
}

#endif