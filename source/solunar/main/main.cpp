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
#include <engine/inputmanager_dinput.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

namespace solunar
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
		g_commandLine.Init(argc, argv);

		Core::Init();

		// create game content device
		g_gameContentDevice = mem_new<ContentDevice>("data", appGetCurrentDir());

		// mount game content device
		g_contentManager->MountDevice(g_gameContentDevice, "game");
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

		g_contentManager->UnmountDevice("game");

		if (g_gameContentDevice)
		{
			mem_delete(g_gameContentDevice);
			g_gameContentDevice = nullptr;
		}

		Core::Shutdown();
	}

	void appInitInput()
	{
		InputManager_DInput* pInputManagerDInput = dynamic_cast<InputManager_DInput*>(InputManager::GetInstance());
		if (pInputManagerDInput)
			pInputManagerDInput->Init(g_engineWindow);
	}

	BOOL g_fMouseInClient;
	BOOL g_fGainedFocus = true;

	void OnLostFocus()
	{
		InputManager::GetInstance()->SetCursorCapture(false);
		InputManager::GetInstance()->SetCursorHiding(false);

		g_fGainedFocus = false;
	}

	void OnGainedFocus()
	{
		g_fGainedFocus = true;

		if (!g_engineData.m_editor)
		{
			if (g_engineData.m_shouldCaptureMouse)
				InputManager::GetInstance()->SetCursorCapture(true);

			if (g_engineData.m_shouldHideMouse)
				InputManager::GetInstance()->SetCursorHiding(true);
		}
	}

	LRESULT CALLBACK wndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		InputManager_Win32* inputManager = (InputManager_Win32*)InputManager::GetInstance();

		switch (Msg)
		{
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

		case WM_KEYDOWN:
		{
			Win32Keys::Keys key = static_cast<Win32Keys::Keys>(wParam);
			inputManager->KeyboardAction(static_cast<int>(getKeyFromWin32(key)), true);
			break;
		}

		case WM_KEYUP:
		{
			Win32Keys::Keys key = static_cast<Win32Keys::Keys>(wParam);
			inputManager->KeyboardAction(static_cast<int>(getKeyFromWin32(key)), false);
			break;
		}

		/////////////////////////////////////////////////////////
		// Mouse Buttons 
		// #TODO: REWRITE PLEASE TO WM_MBUTTONUP / WM_MBUTTONDOWN

		case WM_LBUTTONDOWN:
			inputManager->MouseButtonAction(MOUSE_BUTTON_LEFT, true);
			break;
		case WM_RBUTTONDOWN:
			inputManager->MouseButtonAction(MOUSE_BUTTON_RIGHT, true);
			break;
		case WM_MBUTTONDOWN:
			inputManager->MouseButtonAction(MOUSE_BUTTON_MIDDLE, true);
			break;

		case WM_LBUTTONUP:
			inputManager->MouseButtonAction(MOUSE_BUTTON_LEFT, false);
			break;
		case WM_RBUTTONUP:
			inputManager->MouseButtonAction(MOUSE_BUTTON_RIGHT, false);
			break;
		case WM_MBUTTONUP:
			inputManager->MouseButtonAction(MOUSE_BUTTON_MIDDLE, false);
			break;

		/////////////////////////////////////////////////////////

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
		//	InputManager::GetInstance()->setCursorCapture(false);
		//	break;

		case WM_ACTIVATE:
		{
			if (LOWORD(wParam) == WA_INACTIVE)
				OnLostFocus();
			else
				OnGainedFocus();
			
			break;
		}
		
		default:
			break;
		}

		ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam);

		return DefWindowProcA(hWnd, Msg, wParam, lParam);
	}

	void InputUpdate()
	{
		InputManager* input = InputManager::GetInstance();

		if (g_fGainedFocus)
		{
			if (g_engineData.m_shouldCaptureMouse)
				input->SetCursorCapture(true);

			if (g_engineData.m_shouldHideMouse)
				input->SetCursorHiding(true);
		}
		else
		{
			input->SetCursorCapture(false);
			input->SetCursorHiding(false);
		}
	}

	void CreateEngineView()
	{
		std::string optionsFilename = "GameSettings.ini";
		if (!g_graphicsOptions.loadSettings(optionsFilename))
		{
			g_graphicsOptions.applyDefaultOptions();
			g_graphicsOptions.SaveSettings(optionsFilename);
		}

		// create window
		int width = g_graphicsOptions.m_width, height = g_graphicsOptions.m_height;
		bool fullscreen = g_graphicsOptions.m_fullscreen;
		const char* title = "EngineWindowWin32";//= "Very big game title";
		
		WNDCLASSA wc = {};
		wc.lpszClassName = "EngineWindowClass";
		wc.lpfnWndProc = wndProc;
		wc.hbrBackground = CreateSolidBrush(0x00000000);
		RegisterClassA(&wc);

		RECT rc = { 0, 0, width, height };
		DWORD windowStyle = (WS_OVERLAPPEDWINDOW) ^ (WS_THICKFRAME | WS_MAXIMIZEBOX);
		AdjustWindowRect(&rc, windowStyle, FALSE);

		g_engineWindow = CreateWindowA("EngineWindowClass", title, windowStyle, 0, 0, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, NULL, NULL);
		ShowWindow(g_engineWindow, SW_SHOW);
		UpdateWindow(g_engineWindow);
		SetForegroundWindow(g_engineWindow);

		g_engineView = mem_new<View>();
		g_engineView->m_width = width;
		g_engineView->m_height = height;
		g_engineView->m_fov = 75.0f;
		g_engineView->m_znear = 0.1f;
		g_engineView->m_zfar = 10000.0f;
		g_engineView->updateInternalValues();

		CameraProxy::GetInstance()->SetView(g_engineView);
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

	void MainLoop(EngineLoop* engineLoop)
	{
		MSG Msg = {};
		while (Msg.message != WM_QUIT)
		{
			if (PeekMessageA(&Msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&Msg);
				DispatchMessageA(&Msg);
			}
			else
			{
				InputUpdate();

				if (!engineLoop->Update())
					break;
			}
		}
	}

	int main(int argc, char* argv[])
	{
#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
		//_CrtSetBreakAlloc(11760);
#endif // _DEBUG

		appInit(argc, argv);
		
		EngineLoop engineLoop;
		engineLoop.Initialize();

		MainLoop(&engineLoop);
#if 0
		/*while (!glfwWindowShouldClose(g_engineWindow))
		{
			OPTICK_FRAME("Main Thread");

			if (engineLoop.update())
				break;
		}*/
#endif

		Core::Msg("Quiting from engine loop ...");

		engineLoop.Shutdown();
		
		appShutdown();

		return 0;
	}
}
