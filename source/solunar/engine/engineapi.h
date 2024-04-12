#ifndef ENGINEAPI_H
#define ENGINEAPI_H

#ifdef ENGINE_DLL
#	ifdef ENGINE_COMPILE
#		define ENGINE_API __declspec(dllexport)
#else
#		define ENGINE_API __declspec(dllimport)
#endif // ENGINE_COMPILE
#endif // ENGINE_DLL

#ifndef ENGINE_API
#	define ENGINE_API
#endif // !ENGINE_API

#endif