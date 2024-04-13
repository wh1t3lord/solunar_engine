#ifndef FPSGAME_H
#define FPSGAME_H

#include "engine/gameinterface.h"

#define MSG(s) Core::msg("%s: %s", __FUNCTION__, s)

namespace engine
{

class FPSGameInterface : public IGameInterface
{
public:
	FPSGameInterface();
	~FPSGameInterface();

	void initialize() override;
	void shutdown() override;
};

FPSGameInterface* getShockGameInterface();

void shockGamePlayerDebug(bool* open);

}

#endif // !SHOCKGAME_H
