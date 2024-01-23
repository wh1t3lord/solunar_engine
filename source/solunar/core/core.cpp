#include "core/core.h"
#include "core/utils/logger.h"

#include <exception>
#include <thread>
#include <mutex>

namespace engine
{
	void error_backend(const char* txt)
	{
#ifdef WIN32
		MessageBoxA(0, txt, "Engine Error", MB_OK | MB_ICONERROR);
#endif
	}

	void warning_backend(const char* txt)
	{
#ifdef WIN32
		MessageBoxA(0, txt, "Warning!", MB_OK | MB_ICONWARNING);
#endif
	}

	static std::mutex g_loggerMutex;

	void Core::msg(const char* msg, ...)
	{
		std::lock_guard<std::mutex> lockGuard(g_loggerMutex);

		static char buf[2048 * 2];
		va_list args;

		va_start(args, msg);
		vsnprintf(buf, sizeof(buf), msg, args);
		va_end(args);

		Logger::logPrint("%s", buf);
	}

	void Core::error(const char* msg, ...)
	{
		std::lock_guard<std::mutex> lockGuard(g_loggerMutex);

		static char buf[2048 * 2];
		va_list args;

		va_start(args, msg);
		vsnprintf(buf, sizeof(buf), msg, args);
		va_end(args);

#ifndef MASTER_GOLD_BUILD
		Logger::logPrint("!!! ERROR: %s", buf);
		error_backend(buf);
		std::terminate();
#else
		std::terminate();
#endif // !MASTER_GOLD_BUILD


//#if defined(DEBUG) || defined(_DEBUG)
		
//#else
//		exit(1);
//#endif

		ExitProcess(1);
	}

	void Core::warning(const char* msg, ...)
	{
		static char buf[2048 * 2];
		va_list args;

		va_start(args, msg);
		vsnprintf(buf, sizeof(buf), msg, args);
		va_end(args);

		Logger::logPrint("Warning: %s", buf);

#ifndef MASTER_GOLD_BUILD
		warning_backend(buf);
#endif // !MASTER_GOLD_BUILD

	}

}