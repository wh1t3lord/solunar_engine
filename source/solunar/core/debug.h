#ifndef DEBUG_H
#define DEBUG_H

//#define MASTER_GOLD

#define DEBUG_INFO __FILE__, __LINE__, __FUNCTION__

// TODO: DMan: need to move this because this macroses can be used not for only debug purposes
#define NORETURN __declspec(noreturn)
#define GET_ADDITIONAL_ARGS(fmt, buff) \
	do { va_list p; va_start(p, fmt); vsnprintf(buff, sizeof(buff), fmt, p); va_end(p); } while (0)

namespace engine
{

class Debug
{
public:
	static void initialize();

	static void assertionFailed(
		const char*,
		const char*,
		int,
		const char*,
		const char* message = nullptr,
		const char* arg1 = nullptr,
		const char* arg2 = nullptr,
		const char* arg3 = nullptr
	);

	static void fatal(const char*, int, const char*, const char* fmt, ...);

private:
	static void assertionFailedBackend(const char* buf);
};

}

#ifndef MASTER_GOLD
#define Assert(EXPR) \
	if ( !(EXPR) ) \
		engine::Debug::assertionFailed(#EXPR, DEBUG_INFO, "assertion failed")

#define Assert2(EXPR, ARG1) \
	if ( !(EXPR) ) \
		engine::Debug::assertionFailed(#EXPR, DEBUG_INFO, "assertion failed", ARG1)

#define Assert3(EXPR, ARG1, ARG2) \
	if ( !(EXPR) ) \
		engine::Debug::assertionFailed(#EXPR, DEBUG_INFO, "assertion failed", ARG1, ARG2)

#define Assert4(EXPR, ARG1, ARG2, ARG3) \
	if ( !(EXPR) ) \
		engine::Debug::assertionFailed(#EXPR, DEBUG_INFO, "assertion failed", ARG1, ARG2, ARG3)

#define Fatal(fmt, ...) \
	engine::Debug::fatal(DEBUG_INFO, fmt, __VA_ARGS__)

#ifdef assert
#undef assert
#endif // assert

#define assert Assert
#else
#define Assert(EXPR) ((void)0)
#define Assert2(EXPR, ARG1) ((void)0)
#define Assert3(EXPR, ARG1, ARG2) ((void)0)
#define Assert4(EXPR, ARG1, ARG2, ARG3) ((void)0)
#define Fatal(fmt, ...) \
	engine::Debug::fatal(DEBUG_INFO, fmt, __VA_ARGS__)

#endif // !MASTER_GOLD



#endif // !DEBUG_H
