#include "gamepch.h"
#include "gameframework/gamelogic/world/levelmanager.h"
#include "levelmanager.h"

namespace engine
{

LevelManagerComponent::LevelManagerComponent()
{
}

LevelManagerComponent::~LevelManagerComponent()
{
}

void LevelManagerComponent::registerObject()
{
	g_typeManager->registerObject<LevelManagerComponent>();
}

void LevelManagerComponent::onWorldSet(World* world)
{
	Component::onWorldSet(world);
}
}
