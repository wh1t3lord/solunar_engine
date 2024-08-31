#ifndef SHOCKGAME_H
#define SHOCKGAME_H

#include "engine/gameinterface.h"

namespace solunar
{

class ShockGameInterface : public IGameInterface
{
public:
	ShockGameInterface();
	~ShockGameInterface();

	void Initialize() override;
	void Shutdown() override;
};

ShockGameInterface* getShockGameInterface();

}

#endif // !SHOCKGAME_H
