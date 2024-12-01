#include "common.h"

int sprintf(char* const p_string, const size_t buffer_count, char const* const p_format, ...)
{
	// todo: kirrik -> provide sane preprocessors for determing platform SOLUNAR_PLATFORM_WINDOWS and etc
#ifdef _MSC_VER
	va_list args;
	va_start(args, p_format);
	int result = ::vsprintf_s(p_string, buffer_count, p_format, args);
	va_end(args);
	return result;
#else
#error provide implementation
#endif
}
