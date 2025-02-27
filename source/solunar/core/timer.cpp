#include "core/platform/core_platform.h"
#include "core/timer.h"

namespace solunar
{

Timer* Timer::GetInstance()
{
	static Timer timer;
	return &timer;
}

#ifdef WIN32
Timer::Timer()
{
	m_frequency = 0;
	m_startTime = 0;
	m_endTime = 0;
	m_deltaTime = 0.0f;
}

Timer::~Timer()
{
}

void Timer::Init()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
	m_endTime = m_startTime;
}

void Timer::Update()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
	m_deltaTime = (float)(m_startTime - m_endTime) / m_frequency;


	m_endTime = m_startTime;
}

float Timer::GetDelta()
{
	return m_deltaTime;
}

float Timer::getTime()
{
	INT64 time;
	QueryPerformanceCounter((LARGE_INTEGER*)&time);
	return static_cast<float>(time / m_frequency);
}
#else
Timer::Timer()
{
	m_deltaTime = 0.0f;
}

Timer::~Timer()
{
}

void Timer::Init()
{
	m_startTime = clock();
	m_endTime = clock();
}

void Timer::Update()
{
	m_startTime = clock();
	m_deltaTime = (float)(m_startTime - m_endTime) / CLOCKS_PER_SEC;
	m_endTime = m_startTime;
}

float Timer::GetDelta()
{
	return m_deltaTime;
}

float Timer::getTime()
{
	clock_t time = clock();
	return static_cast<float>(time / CLOCKS_PER_SEC);
}
#endif // WIN32
}
