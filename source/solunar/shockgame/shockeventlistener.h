#ifndef SHOCKEVENTLISTENER_H
#define SHOCKEVENTLISTENER_H

#if 0

#include "engine/eventmanager.h"

namespace solunar
{

class ShockEventListener : public IEventListener
{
public:
	ShockEventListener();
	~ShockEventListener();

	void processEvent(const EventDesc& eventDesc) override;
};

extern ShockEventListener g_shockEventListener;

}

#endif

#endif // !SHOCKEVENTLISTENER_H
