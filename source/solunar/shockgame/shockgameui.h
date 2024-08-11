#ifndef SHOCKGAME_SHOCKGAMEUI_H
#define SHOCKGAME_SHOCKGAMEUI_H

#include "engine/entity/logiccomponent.h"

namespace solunar
{

class ShockGameMainMenuComponent : public LogicComponent
{
	IMPLEMENT_OBJECT(ShockGameMainMenuComponent, LogicComponent);

public:
	ShockGameMainMenuComponent();
	~ShockGameMainMenuComponent();

	void OnWorldSet(World* world) override;

	void Update(float dt) override;
};

class DemoGameMainMenuComponent : public LogicComponent
{
	IMPLEMENT_OBJECT(DemoGameMainMenuComponent, LogicComponent);

public:
	DemoGameMainMenuComponent();
	~DemoGameMainMenuComponent();

	void OnWorldSet(World* world) override;

	void Update(float dt) override;
};

}

#endif // !SHOCKGAME_SHOCKGAMEUI_H
