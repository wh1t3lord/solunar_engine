#include "enginepch.h"
#include "main/main.h"
#include "engine/inputmanager_dinput.h"

#pragma comment(lib, "dinput8.lib")

namespace solunar
{
	InputManager_DInput::InputManager_DInput() :
		m_pDirectInput(nullptr),
		m_pDIKeyboard(nullptr),
		m_lastCursorHiding(false)
	{
	}

	InputManager_DInput::~InputManager_DInput()
	{
	}

	void InputManager_DInput::Init(HWND hWindow)
	{
		Core::Msg("InputManager_DInput: Initializing DirectInput ...");

		HRESULT hr = DirectInput8Create(GetModuleHandleA(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
		if (FAILED(hr))
			Core::Error("Failed to initialize DirectInput. Error: 0x%l", hr);

		hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pDIKeyboard, NULL);
		if FAILED(hr)
			Core::Error("Failed to create DirectInput device. Error: 0x%l", hr);

		m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
		m_pDIKeyboard->SetCooperativeLevel(hWindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		m_pDIKeyboard->Acquire();
	}

	void InputManager_DInput::Shutdown()
	{
		if (m_pDIKeyboard)
		{
			m_pDIKeyboard->Unacquire();
			m_pDIKeyboard->Release();
			m_pDIKeyboard = nullptr;
		}

		if (m_pDirectInput)
		{
			m_pDirectInput->Release();
			m_pDirectInput = nullptr;
		}
	}

	void InputManager_DInput::Update()
	{
		InputManager::Update();

		HRESULT hr = m_pDIKeyboard->GetDeviceState(sizeof(m_keys), (LPVOID)&m_keys);
		if FAILED(hr)
		{
			// If it failed, the device has probably been lost. Attempt to reacquire it. 
			if (hr == DIERR_INPUTLOST)
				ReacquireDevices();

			return;
		}

		if (m_lastCursorHiding != m_hideCursor)
		{
			if (m_hideCursor)
				::ShowCursor(FALSE);
			else
				::ShowCursor(TRUE);

			m_lastCursorHiding = m_hideCursor;
		}

		if (m_captureCursor)
		{
			RECT rect = {};
			::GetClientRect((HWND)appGetWindow(), &rect);

			int width = rect.right - rect.left;
			int height = rect.bottom - rect.top;

			int halfWidth = width / 2;
			int halfHeight = height / 2;

			POINT pt = {};
			::GetCursorPos(&pt);
			
			::SetCursorPos(halfWidth, halfHeight);

			pt.x = pt.x - halfWidth;
			pt.y = pt.y - halfHeight;

			m_cursorPos.x = static_cast<float>(pt.x);
			m_cursorPos.y = static_cast<float>(pt.y);

			m_deltaCursorPos.x = static_cast<float>(pt.x);
			m_deltaCursorPos.y = static_cast<float>(-pt.y);

			m_lastCursorPos = m_cursorPos;

		}
		else
		{
			ResetDelta();
		}
	}

	void InputManager_DInput::KeyboardAction(uint32_t keyId, bool state)
	{
		if (keyId >= 420)
			return;

		m_keys[keyId] = state;
	}

	void InputManager_DInput::MouseButtonAction(MouseButtons buttonId, bool state)
	{
		Assert(buttonId <= MOUSE_BUTTON_COUNT);
		m_mouseButtons[buttonId] = state;
	}

	void InputManager_DInput::ReacquireDevices()
	{
		if (m_pDIKeyboard)
		{
			m_pDIKeyboard->Unacquire();
			m_pDIKeyboard->Acquire();
		}
	}

}