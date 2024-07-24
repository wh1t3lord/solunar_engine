#include "pch.h"
#include "main/main.h"
#include "main/engineloop.h"
#include "core/memory/memorymanager.h"
#include "core/timer.h"
#include "core/file/filesystem.h"
#include "core/file/contentdevice.h"
#include "core/file/contentmanager.h"

#include "engine/engine.h"
#include "engine/camera.h"

#include "graphics/graphicsoptions.h"
#include "graphics/view.h"

#include "main/win32_keys.h"

#include "backends/imgui_impl_win32.h"
#include <engine/inputmanager_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

	BOOL g_fMouseInClient;
	BOOL g_fGainedFocus = true;

	void onLostFocus()
	{
		InputManager::getInstance()->setCursorCapture(false);
		InputManager::getInstance()->setCursorHiding(false);

		g_fGainedFocus = false;
	}

	void onGainedFocus()
	{
		g_fGainedFocus = true;

		if (g_engineData.m_shouldCaptureMouse)
			InputManager::getInstance()->setCursorCapture(true);

		if (g_engineData.m_shouldHideMouse)
			InputManager::getInstance()->setCursorHiding(true);
	}

	LRESULT CALLBACK wndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		InputManager_Win32* inputManager = (InputManager_Win32*)InputManager::getInstance();

		switch (Msg)
		{
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

		case WM_KEYDOWN:
		{
			Win32Keys::Keys key = static_cast<Win32Keys::Keys>(wParam);
			inputManager->keyboardAction(static_cast<int>(getKeyFromWin32(key)), true);
			break;
		}

		case WM_KEYUP:
		{
			Win32Keys::Keys key = static_cast<Win32Keys::Keys>(wParam);
			inputManager->keyboardAction(static_cast<int>(getKeyFromWin32(key)), false);
			break;
		}

		case WM_MOUSEMOVE:
		{
			if (!g_fMouseInClient) {
				g_fMouseInClient = TRUE;
				TRACKMOUSEEVENT tme = { sizeof(tme) };
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = hWnd;
				TrackMouseEvent(&tme);

				//SetCapture(hWnd);
			}
			break;
		}

		case WM_MOUSELEAVE:
		{
			// #TODO: For menu
			//ReleaseCapture();

			//SetCursorPos(0, 0);

			g_fMouseInClient = FALSE;
			return 0;
		}

		//case WM_KILLFOCUS:
		//	InputManager::getInstance()->setCursorCapture(false);
		//	break;

		case WM_ACTIVATE:
		{
			if (LOWORD(wParam) == WA_INACTIVE)
				onLostFocus();
			else
				onGainedFocus();
			
			break;
		}
		
		default:
			break;
		}

		ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam);

		return DefWindowProcA(hWnd, Msg, wParam, lParam);
	}

	void inputUpdate()
	{
		if (g_fGainedFocus)
		{
			if (g_engineData.m_shouldCaptureMouse)
				InputManager::getInstance()->setCursorCapture(true);

			if (g_engineData.m_shouldHideMouse)
				InputManager::getInstance()->setCursorHiding(true);
		}
	}

	void createEngineView()
	{
		std::string optionsFilename = "GameSettings.ini";
		if (!g_graphicsOptions.loadSettings(optionsFilename))
		{
			g_graphicsOptions.applyDefaultOptions();
			g_graphicsOptions.saveSettings(optionsFilename);
		}

		// create window
		int width = g_graphicsOptions.m_width, height = g_graphicsOptions.m_height;
		bool fullscreen = g_graphicsOptions.m_fullscreen;
#ifndef FINAL_BUILD
		const char* title = "Vengeance Compile date: " __TIMESTAMP__;//= "Very big game title";
#else
		const char* title = "Project Vengeance";//= "Very big game title";
#endif // !FINAL_BUILD
		
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
				inputUpdate();

				if (!engineLoop->update())
					break;
			}
		}
	}

	int main(int argc, char* argv[])
	{
#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
		//_CrtSetBreakAlloc(1335);
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
