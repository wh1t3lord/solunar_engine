#include "core/timer.h"

namespace engine
{

#ifdef WIN32
Timer* Timer::getInstance()
{
	static Timer timer;
	return &timer;
}

Timer::Timer()
{
	m_deltaTime = 0.0f;
}

Timer::~Timer()
{
}

void Timer::init()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
	m_endTime = m_startTime;
}

void Timer::update()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
	m_deltaTime = (float)(m_startTime - m_endTime) / m_frequency;

	// #TODO: rework timer
	if (m_deltaTime > 0.5f)
		m_deltaTime = 0.5f;

	m_endTime = m_startTime;
}

float Timer::getDelta()
{
	return m_deltaTime;
}

float Timer::getTime()
{
	INT64 time;
	QueryPerformanceCounter((LARGE_INTEGER*)&time);
	return static_cast<float>(time / m_frequency);
}
#endif // WIN32
}
