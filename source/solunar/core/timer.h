#ifndef TIMER_H
#define TIMER_H

#include "core/core.h"

namespace solunar
{

#ifdef WIN32
class Timer
{
public:
	static Timer* GetInstance();

public:
	Timer();
	~Timer();

	void Init();
	
	void Update();

	float getDelta();
	float getTime();

private:
	INT64 m_frequency;
	INT64 m_startTime;
	INT64 m_endTime;

	float m_deltaTime;
};
#else
class Timer
{
public:
	static Timer* GetInstance();

public:
	Timer();
	~Timer();

	void Init();

	void Update();

	float getDelta();
	float getTime();
private:
	clock_t m_startTime;
	clock_t m_endTime;
	float m_deltaTime;
};
#endif // WIN32

}

#endif // !TIMER_H
