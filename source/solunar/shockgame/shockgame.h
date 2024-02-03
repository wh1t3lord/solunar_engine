#ifndef SHOCKGAME_H
#define SHOCKGAME_H

#include "engine/gameinterface.h"

namespace engine
{

class ShockGameInterface : public IGameInterface
{
public:
	ShockGameInterface();
	~ShockGameInterface();

	void initialize() override;
	void shutdown() override;
};

ShockGameInterface* getShockGameInterface();

void shockGamePlayerDebug(bool* open);

}

#endif // !SHOCKGAME_H
