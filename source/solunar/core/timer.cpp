#include "core/timer.h"

namespace engine
{

Timer* Timer::getInstance()
{
	static Timer timer;
	return &timer;
}

#ifdef WIN32
void Timer::init()
{
	QueryPerformanceFrequency(&m_frequency);
	m_floatFrequency = (float)m_frequency.QuadPart;

	QueryPerformanceCounter(&m_startTime);
	m_startTime = m_endTime;
}

void Timer::update()
{
	QueryPerformanceCounter(&m_startTime);
	m_deltaTime = static_cast<float>(m_startTime.QuadPart - m_endTime.QuadPart) / m_frequency.QuadPart;
	m_endTime = m_startTime;
}

float Timer::getDelta()
{
	return m_deltaTime;
}

float Timer::getTime()
{
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	return static_cast<float>(time.QuadPart / m_frequency.QuadPart);
}
#endif // WIN32
}
