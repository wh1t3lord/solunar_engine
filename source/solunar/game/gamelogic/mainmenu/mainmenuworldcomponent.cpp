#include "gamepch.h"
#include "game/gamelogic/mainmenu/mainmenuworldcomponent.h"

#include "engine/entity/world.h"

namespace solunar {
	
	void MainMenuWorldComponent::RegisterObject()
	{
		g_typeManager->RegisterObject<MainMenuWorldComponent>();
	}
	
	MainMenuWorldComponent::MainMenuWorldComponent()
	{
	}
	
	MainMenuWorldComponent::~MainMenuWorldComponent()
	{	
	}
	
	void MainMenuWorldComponent::OnWorldSet(World* world)
	{
		Component::OnWorldSet(world);
		
		// Initialize game state to draw menu and Release mouse pointer
		// GameState::GetInstance()->setGameState(GameState::GAME_STATE_MAIN_MENU);
	}

}