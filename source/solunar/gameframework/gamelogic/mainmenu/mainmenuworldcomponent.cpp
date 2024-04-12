#include "gamepch.h"
#include "game/gamelogic/mainmenu/mainmenuworldcomponent.h"

#include "engine/entity/world.h"

namespace engine {
	
	void MainMenuWorldComponent::registerObject()
	{
		g_typeManager->registerObject<MainMenuWorldComponent>();
	}
	
	MainMenuWorldComponent::MainMenuWorldComponent()
	{
	}
	
	MainMenuWorldComponent::~MainMenuWorldComponent()
	{	
	}
	
	void MainMenuWorldComponent::onWorldSet(World* world)
	{
		Component::onWorldSet(world);
		
		// Initialize game state to draw menu and release mouse pointer
		// GameState::getInstance()->setGameState(GameState::GAME_STATE_MAIN_MENU);
	}

}