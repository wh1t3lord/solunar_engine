#include "core/core.h"
#include "core/utils/logger.h"

#include <exception>
#include <thread>
#include <mutex>

namespace solunar
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

	void Core::Msg(const char* msg, ...)
	{
		//std::lock_guard<std::mutex> lockGuard(g_loggerMutex);

		static char buf[2048 * 2];
		va_list args;

		va_start(args, msg);
		vsnprintf(buf, sizeof(buf), msg, args);
		va_end(args);

		Logger::LogPrint("%s", buf);
	}

	void Core::Error(const char* msg, ...)
	{
		//std::lock_guard<std::mutex> lockGuard(g_loggerMutex);

		static char buf[2048 * 2];
		va_list args;

		va_start(args, msg);
		vsnprintf(buf, sizeof(buf), msg, args);
		va_end(args);

#ifndef FINAL_BUILD
		Logger::LogPrint("!!! ERROR: %s", buf);
		error_backend(buf);
		std::terminate();
#else
		Logger::LogPrint("!!! ERROR: %s", buf);
		error_backend(buf);
#endif // !MASTER_GOLD_BUILD

#ifdef _WINDOWS
		ExitProcess(1);
#else
		exit(1);
#endif
	}

	void Core::Warning(const char* msg, ...)
	{
		static char buf[2048 * 2];
		va_list args;

		va_start(args, msg);
		vsnprintf(buf, sizeof(buf), msg, args);
		va_end(args);

		Logger::LogPrint("Warning: %s", buf);

#ifndef MASTER_GOLD_BUILD
		warning_backend(buf);
#endif // !MASTER_GOLD_BUILD

	}

}