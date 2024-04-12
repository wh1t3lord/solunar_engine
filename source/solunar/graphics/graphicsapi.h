#ifndef GRAPHICSAPI_H
#define GRAPHICSAPI_H

#ifdef ENGINE_DLL
#	ifdef GRAPHICS_COMPILE
#		define GRAPHICS_API __declspec(dllexport)
#else
#		define GRAPHICS_API __declspec(dllimport)
#endif // GRAPHICS_COMPILE
#endif // ENGINE_DLL

#ifndef GRAPHICS_API
#	define GRAPHICS_API
#endif // !GRAPHICS_API

#endif