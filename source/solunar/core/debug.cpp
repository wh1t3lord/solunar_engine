#include "core/core.h"
#include "core/debug.h"

using namespace solunar;

#ifdef _MSC_VER
#include <Windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "DbgHelp.lib")

LONG WINAPI UnhandleExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	char executableName[256];
	GetModuleFileNameA(NULL, executableName, 256);

	char minidumpPath[256];
	snprintf(minidumpPath, sizeof(minidumpPath), "%s_%i-%i-%i_%i-%i-%i-%i.dmp",
		executableName, time.wYear, time.wMonth, time.wDay,
		time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	HANDLE minidumpFile = CreateFileA(minidumpPath, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (minidumpFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION minidumpInfo;
		minidumpInfo.ThreadId = GetCurrentThreadId();
		minidumpInfo.ExceptionPointers = ExceptionInfo;
		minidumpInfo.ClientPointers = FALSE;

		DWORD minidumpType =
			MiniDumpNormal			|
			MiniDumpFilterMemory	|
			MiniDumpScanMemory		|
			MiniDumpWithDataSegs	|
			MiniDumpWithIndirectlyReferencedMemory;

		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), minidumpFile,
			(MINIDUMP_TYPE)minidumpType, &minidumpInfo, 0, 0);

		CloseHandle(minidumpFile);
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

static struct DebugInitializer
{
	DebugInitializer()
	{
		Debug::initialize();
	}
} sDebugInitializer;

void Debug::initialize()
{
	if (!IsDebuggerPresent())
		SetUnhandledExceptionFilter(UnhandleExceptionFilter);

	//SymInitialize(GetCurrentProcess(), )
}
#else
void Debug::initialize()
{

}
#endif // _MSC_VER

static char errorInfoBuffer[4096];
static const char* MESSAGE_FORMAT =
"Fatal Error!\r\n\
\r\n\
\r\n\
Expression: %s\r\n\
File: %s\r\n\
Line: %i\r\n\
Function: %s\
";

void Debug::assertionFailed(const char* expression, const char* file, int line, const char* function, const char* message, const char* arg1, const char* arg2, const char* arg3)
{
	sprintf(
		errorInfoBuffer,
		MESSAGE_FORMAT,
		expression,
		file,
		line,
		function
	);
	
	if (message != NULL)
	{
		strcat(errorInfoBuffer, "\r\nDescription: "); strcat(errorInfoBuffer, message);
	}
	if (arg1 != NULL)
	{
		strcat(errorInfoBuffer, "\r\nArgument 1: "); strcat(errorInfoBuffer, arg1);
	}
	if (arg2 != NULL)
	{
		strcat(errorInfoBuffer, "\r\nArgument 2: "); strcat(errorInfoBuffer, arg2);
	}
	if (arg3 != NULL)
	{
		strcat(errorInfoBuffer, "\r\nArgument 3: "); strcat(errorInfoBuffer, arg3);
	}

	assertionFailedBackend(errorInfoBuffer);
}

void Debug::fatal(const char* file, int line, const char* function, const char* fmt, ...)
{
	char buff[1024]; GET_ADDITIONAL_ARGS(fmt, buff);
	assertionFailed("<no expression>", file, line, function, buff);
}

void Debug::assertionFailedBackend(const char* buf)
{
	solunar::Core::msg("===================================");
	solunar::Core::msg("%s", buf);
	solunar::Core::msg("===================================");

#ifdef WIN32
	int result = MessageBoxA(0, buf, "Fatal Error!", MB_CANCELTRYCONTINUE | MB_ICONERROR);
	switch (result)
	{
	case IDCANCEL: exit(1); break;
	case IDTRYAGAIN: DebugBreak(); break;
	case IDCONTINUE: return;
	}
#endif
}
