#include "core/platform/core_platform.h"
#include "core/utils/logger.h"

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <memory.h>
#include <time.h>

namespace solunar
{
	static FILE* s_logFileHandle = nullptr;
	static clock_t s_logOpenTime = 0;

	void logWriteMsg(const char* msg)
	{
		clock_t dwCurrentTime = clock() - s_logOpenTime;

		static char buffer[1024];
		int len = snprintf(buffer, sizeof(buffer), "%02lu:%02lu.%02lu: %s",
			dwCurrentTime / 60000, dwCurrentTime / 1000 % 60, dwCurrentTime % 1000 / 10, msg);

		if (s_logFileHandle)
		{
			fwrite(buffer, sizeof(char), len, s_logFileHandle);
			fflush(s_logFileHandle);
		}
	}

	void Logger::Init()
	{
		s_logFileHandle = fopen("engine_output.txt", "w");
		assert(s_logFileHandle);

		time_t aclock;
		time(&aclock);
		struct tm* newtime = localtime(&aclock);
		const char* buffer = asctime(newtime);

		char timeBuffer[1024];
		int len = snprintf(timeBuffer, sizeof(timeBuffer), "Log started: %s", buffer);

		fwrite(timeBuffer, sizeof(char), len, s_logFileHandle);

		s_logOpenTime = clock();
	}

	void Logger::Shutdown()
	{
		if (s_logFileHandle)
		{
			fclose(s_logFileHandle);
			s_logFileHandle = nullptr;
		}
	}

	void Logger::LogPrint(const char* Msg, ...)
	{
		static char buffer[2048 * 2];
		va_list args;
		va_start(args, Msg);
		vsnprintf(buffer, sizeof(buffer), Msg, args);
		va_end(args);

		strcat(buffer, "\n");

#ifdef WIN32
		OutputDebugStringA(buffer);
#endif // WIN32
		
		// write to console
		fwrite(buffer, sizeof(char), strlen(buffer), stdout);

		logWriteMsg(buffer);
	}

}
