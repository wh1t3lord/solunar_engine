#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

namespace solunar
{

class IGameInterface
{
public:
	virtual ~IGameInterface() {}

	virtual void initialize() = 0;
	virtual void shutdown() = 0;
};

extern IGameInterface* g_gameInterface;

}

#endif // !GAMEINTERFACE_H
