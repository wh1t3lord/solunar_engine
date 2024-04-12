#ifndef CORE_H
#define CORE_H

#include "core/platform/core_platform.h"

namespace engine
{
	class CORE_API Core
	{
	public:
		static void init();
		static void shutdown();

		static void msg(const char* msg, ...);
		static void error(const char* msg, ...);
		static void warning(const char* msg, ...);
	};
}

#endif // !CORE_H
