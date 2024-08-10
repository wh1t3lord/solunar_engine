#include "shockgamepch.h"
#include "shockgame/shockeventlistener.h"

#if 0

namespace solunar
{

ShockEventListener g_shockEventListener;

ShockEventListener::ShockEventListener()
{
}

ShockEventListener::~ShockEventListener()
{
}

void ShockEventListener::processEvent(const EventDesc& eventDesc)
{
	if (eventDesc.m_eventType == EventType::LoadWorld)
	{
		// Loading screen
		Core::msg("[shockgame]: Switching to loading screen for level %s", eventDesc.m_eventData.c_str());
	}
}

}

#endif