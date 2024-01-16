#include "core/platform/core_platform.h"
#include "core/utils/sysevent.h"

namespace engine {

#ifdef WIN32
	SysEvent::SysEvent(bool manualReset /*= false*/)
	{
		m_eventHandle = (EventHandle_t)CreateEventA(NULL, manualReset, FALSE, NULL);
	}

	SysEvent::~SysEvent()
	{
		if (m_eventHandle != INVALID_HANDLE_VALUE)
			CloseHandle((HANDLE)m_eventHandle);
	}

	void SysEvent::raise()
	{
		SetEvent((HANDLE)m_eventHandle);
	}

	void SysEvent::clear()
	{
		ResetEvent((HANDLE)m_eventHandle);
	}

	bool SysEvent::wait(int timeout /*= kWaitInfinite*/)
	{
		DWORD dwResult = WaitForSingleObject((HANDLE)m_eventHandle, timeout == kWaitInfinite ? INFINITE : timeout);
		return dwResult == WAIT_OBJECT_0;
	}
#endif // WIN32

}