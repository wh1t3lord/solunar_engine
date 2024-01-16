#ifndef MAINMENU_WORLDCOMPONENT_H
#define MAINMENU_WORLDCOMPONENT_H

#include "engine/entity/logiccomponent.h"

namespace engine {
	
	class MainMenuWorldComponent : public LogicComponent
	{
		ImplementObject(MainMenuWorldComponent, LogicComponent);
	public:
		static void registerObject();
		
	public:
		MainMenuWorldComponent();
		~MainMenuWorldComponent();
		
		// Component inheritance
		void onWorldSet(World* world);
	};
	
}

#endif