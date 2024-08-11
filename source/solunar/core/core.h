#ifndef CORE_H
#define CORE_H

#include "core/platform/core_platform.h"

namespace solunar
{
	class Core
	{
	public:
		static void Init();
		static void Shutdown();

		static void Msg(const char* msg, ...);
		static void Error(const char* msg, ...);
		static void Warning(const char* msg, ...);
	};
}

#endif // !CORE_H
