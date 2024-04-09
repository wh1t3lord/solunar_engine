#ifndef FPSGAME_H
#define FPSGAME_H

#include "engine/gameinterface.h"

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
