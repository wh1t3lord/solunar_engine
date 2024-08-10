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
	namespace
	{
		static FILE* s_logFileHandle = nullptr;
		static DWORD s_logOpenTime = 0;

		void logWriteMsg(const char* msg)
		{
			DWORD dwCurrentTime = GetTickCount() - s_logOpenTime;

			static char buffer[1024];
			int len = snprintf(buffer, sizeof(buffer), "%02lu:%02lu.%02lu: %s",
				dwCurrentTime / 60000, dwCurrentTime / 1000 % 60, dwCurrentTime % 1000 / 10, msg);

			if (s_logFileHandle)
			{
				fwrite(buffer, sizeof(char), len, s_logFileHandle);
				fflush(s_logFileHandle);
			}
		}
	}

	void Logger::init()
	{
		s_logFileHandle = fopen("engine_output.txt", "w");
		assert(s_logFileHandle);

		struct tm newtime;
		__time32_t aclock;

		char buffer[32];
		_time32(&aclock);
		_localtime32_s(&newtime, &aclock);
		errno_t errNum = asctime_s(buffer, 32, &newtime);

		char timeBuffer[1024];
		int len = snprintf(timeBuffer, sizeof(timeBuffer), "Log started: %s", buffer);

		fwrite(timeBuffer, sizeof(char), len, s_logFileHandle);

		s_logOpenTime = GetTickCount();
	}

	void Logger::shutdown()
	{
		if (s_logFileHandle)
		{
			fclose(s_logFileHandle);
			s_logFileHandle = nullptr;
		}
	}

	void Logger::logPrint(const char* msg, ...)
	{
		static char buffer[2048 * 2];
		va_list args;
		va_start(args, msg);
		vsnprintf(buffer, sizeof(buffer), msg, args);
		va_end(args);

		strcat(buffer, "\n");

		OutputDebugStringA(buffer);

		// write to console
		fwrite(buffer, sizeof(char), strlen(buffer), stdout);

		logWriteMsg(buffer);
	}

}
