#ifndef TIMER_H
#define TIMER_H

#include "core/core.h"

namespace engine
{

#ifdef WIN32
class Timer
{
public:
	static Timer* getInstance();

public:
	void init();
	
	void update();

	float getDelta();
	float getTime();

private:
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_startTime;
	LARGE_INTEGER m_endTime;

	float m_floatFrequency;
	float m_deltaTime;
};
#endif // WIN32

}

#endif // !TIMER_H
