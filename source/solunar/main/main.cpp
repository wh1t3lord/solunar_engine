#include "pch.h"
#include "main/main.h"
#include "main/engineloop.h"
#include "core/memory/memorymanager.h"
#include "core/timer.h"
#include "core/file/filesystem.h"
#include "core/file/contentdevice.h"
#include "core/file/contentmanager.h"

#include "engine/camera.h"

#include "graphics/graphicsoptions.h"
#include "graphics/view.h"

#include "main/win32_keys.h"

namespace engine
{
	View* g_engineView = nullptr;
	HWND g_engineWindow = nullptr;
	ContentDevice* g_gameContentDevice = nullptr;

	std::string appGetCurrentDir()
	{
#ifdef _MSC_VER
		char buffer[256];
		GetCurrentDirectoryA(256, buffer);

		return buffer;
#endif // _MSC_VER
	}

	void appInit(int argc, char** argv)
	{
		g_commandLine.init(argc, argv);

		Core::init();

		// create game content device
		g_gameContentDevice = mem_new<ContentDevice>("data");

		// mount game content device
		g_contentManager->mountDevice(g_gameContentDevice, "game");
	}

	void appShutdown()
	{
		if (g_engineView)
		{
			mem_delete(g_engineView);
			g_engineView = nullptr;
		}

		if (g_engineWindow)
		{
			DestroyWindow(g_engineWindow);
			g_engineWindow = nullptr;
		}

		UnregisterClassA("EngineWindowClass", NULL);

		g_contentManager->unountDevice("game");

		if (g_gameContentDevice)
		{
			mem_delete(g_gameContentDevice);
			g_gameContentDevice = nullptr;
		}

		Core::shutdown();
	}

	LRESULT CALLBACK wndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		switch (Msg)
		{
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

		case WM_KEYDOWN:
		{
			Win32Keys::Keys key = static_cast<Win32Keys::Keys>(wParam);
			InputManager::getInstance()->keyboardAction(static_cast<int>(getKeyFromWin32(key)), true);
			break;
		}

		case WM_KEYUP:
		{
			Win32Keys::Keys key = static_cast<Win32Keys::Keys>(wParam);
			InputManager::getInstance()->keyboardAction(static_cast<int>(getKeyFromWin32(key)), false);
			break;
		}

		default:
			break;
		}

		return DefWindowProcA(hWnd, Msg, wParam, lParam);
	}

	void createEngineView()
	{
		std::string optionsFilename = "engine.ini";
		if (!g_graphicsOptions.loadSettings(optionsFilename))
		{
			g_graphicsOptions.applyDefaultOptions();
			g_graphicsOptions.saveSettings(optionsFilename);
		}

		// create window
		int width = g_graphicsOptions.m_width, height = g_graphicsOptions.m_height;
		bool fullscreen = g_graphicsOptions.m_fullscreen;
		const char* title = "Very big game title";

		WNDCLASSA wc = {};
		wc.lpszClassName = "EngineWindowClass";
		wc.lpfnWndProc = wndProc;
		RegisterClassA(&wc);

		RECT rc = { 0, 0, width, height };
		DWORD windowStyle = (WS_OVERLAPPEDWINDOW) ^ (WS_THICKFRAME | WS_MAXIMIZEBOX);
		AdjustWindowRect(&rc, windowStyle, FALSE);

		g_engineWindow = CreateWindowA("EngineWindowClass", title, windowStyle, 0, 0, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, NULL, NULL);
		ShowWindow(g_engineWindow, SW_SHOW);
		UpdateWindow(g_engineWindow);

		g_engineView = mem_new<View>();
		g_engineView->m_width = width;
		g_engineView->m_height = height;
		g_engineView->m_fov = 75.0f;
		g_engineView->m_znear = 0.1f;
		g_engineView->m_zfar = 10000.0f;
		g_engineView->updateInternalValues();

		CameraProxy::getInstance()->setView(g_engineView);
	}

	void appPresent()
	{
		// glfwSwapBuffers(g_engineWindow);
	}

	void* appGetWindow()
	{
		return g_engineWindow;
	}

	View* appGetView()
	{
		return g_engineView;
	}

	void mainLoop(EngineLoop* engineLoop)
	{
		MSG msg = {};
		while (msg.message != WM_QUIT)
		{
			if (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessageA(&msg);
			}
			else
			{
				if (!engineLoop->update())
					break;
			}
		}
	}

	int main(int argc, char* argv[])
	{
#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
		//	_CrtSetBreakAlloc(3080);
#endif // _DEBUG

		appInit(argc, argv);
		
		EngineLoop engineLoop;
		engineLoop.initialize();

		mainLoop(&engineLoop);
#if 0
		/*while (!glfwWindowShouldClose(g_engineWindow))
		{
			OPTICK_FRAME("Main Thread");

			if (engineLoop.update())
				break;
		}*/
#endif

		Core::msg("Quiting from engine loop ...");

		engineLoop.shutdown();
		
		appShutdown();

		return 0;
	}
}
