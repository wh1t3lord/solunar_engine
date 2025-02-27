#include "gamepch.h"
#include "game/gamelogic/world/levelmanager.h"
#include "levelmanager.h"

namespace solunar
{

IMPLEMENT_OBJECT(LevelManagerComponent, Component);

LevelManagerComponent::LevelManagerComponent()
{
}

LevelManagerComponent::~LevelManagerComponent()
{
}

void LevelManagerComponent::RegisterObject()
{
	g_typeManager->RegisterObject<LevelManagerComponent>();
}

void LevelManagerComponent::OnWorldSet(World* world)
{
	Component::OnWorldSet(world);
}
}
