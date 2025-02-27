#ifndef THREAD_H
#define THREAD_H

namespace solunar
{

#ifdef _WIN32
typedef unsigned ThreadId;
#else
typedef unsigned ThreadId;
#endif // _WIN32

class Thread
{
public:
	Thread();
	virtual ~Thread();

	bool start();
	void setThreadName(const char* name);

	void Stop();

	virtual void execute() = 0;

	bool IsRunning() { return m_isRunning; }

private:
	void* m_threadHandle;
	ThreadId m_threadId;
	bool m_isRunning;
};

void setCurrentThreadName(const char* name);

} 

#endif