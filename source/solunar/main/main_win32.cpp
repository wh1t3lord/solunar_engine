#include "pch.h"
#include "main.h"

HANDLE g_semaphore;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	g_semaphore = CreateSemaphoreA(NULL, 1, 1, "engine_semaphore");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return 1;

	return solunar::main(__argc, __argv);
}