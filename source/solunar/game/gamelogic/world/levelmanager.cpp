#include "gamepch.h"
#include "game/gamelogic/world/levelmanager.h"
#include "levelmanager.h"

namespace solunar
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
