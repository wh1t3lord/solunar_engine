#ifndef GAMEAPI_H
#define GAMEAPI_H

#ifdef GAME_DLL
#	ifdef GAME_COMPILE
#		define GAME_API __declspec(dllexport)
#else
#		define GAME_API __declspec(dllimport)
#endif // GAME_COMPILE
#endif // GAME_DLL

#ifndef GAME_API
#	define GAME_API
#endif // !GAME_API

#endif