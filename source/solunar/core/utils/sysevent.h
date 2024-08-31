#ifndef COMMON_SYSEVENT_H
#define COMMON_SYSEVENT_H

namespace solunar {

	typedef void* EventHandle_t;

	//! System event wrapper
	class SysEvent
	{
	public:
		static const int kWaitInfinite = -1;

	public:
		SysEvent(bool manualReset = false);
		~SysEvent();

		void raise();
		void clear();

		bool wait(int timeout = kWaitInfinite);

	private:
		EventHandle_t m_eventHandle;
	};

}

#endif // !COMMON_SYSEVENT_H
