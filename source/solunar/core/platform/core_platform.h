#ifndef CORE_PLATFORM_H
#define CORE_PLATFORM_H

#ifdef WIN32
#	include "core/platform/core_platform_win32.h"
#endif // WIN32

#ifdef CORE_DLL
#	ifdef CORE_COMPILE
#		define CORE_API __declspec(dllexport)
#else
#		define CORE_API __declspec(dllimport)
#endif // CORE_COMPILE
#endif // CORE_DLL

#ifndef CORE_API
#	define CORE_API
#endif // !CORE_API

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <memory.h>
#include <math.h>

#endif // !CORE_PLATFORM_H
