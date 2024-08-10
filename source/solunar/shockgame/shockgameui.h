#ifndef SHOCKGAME_SHOCKGAMEUI_H
#define SHOCKGAME_SHOCKGAMEUI_H

#include "engine/entity/logiccomponent.h"

namespace solunar
{

class ShockGameMainMenuComponent : public LogicComponent
{
	ImplementObject(ShockGameMainMenuComponent, LogicComponent);

public:
	ShockGameMainMenuComponent();
	~ShockGameMainMenuComponent();

	void onWorldSet(World* world) override;

	void update(float dt) override;
};

class DemoGameMainMenuComponent : public LogicComponent
{
	ImplementObject(DemoGameMainMenuComponent, LogicComponent);

public:
	DemoGameMainMenuComponent();
	~DemoGameMainMenuComponent();

	void onWorldSet(World* world) override;

	void update(float dt) override;
};

}

#endif // !SHOCKGAME_SHOCKGAMEUI_H
