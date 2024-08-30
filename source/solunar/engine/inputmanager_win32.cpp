#include "enginepch.h"
#include "main/main.h"
#include "engine/inputmanager_win32.h"

namespace solunar
{
	InputManager_Win32 InputManager_Win32::ms_inputManager;

	InputManager_Win32::InputManager_Win32() :
		m_lastCursorHiding(false)
	{
	}

	InputManager_Win32::~InputManager_Win32()
	{
	}

	void InputManager_Win32::Update()
	{
		InputManager::Update();

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

	void InputManager_Win32::KeyboardAction(uint32_t keyId, bool state)
	{
		if (keyId >= 420)
			return;

		m_keys[keyId] = state;
	}

	void InputManager_Win32::MouseButtonAction(MouseButtons buttonId, bool state)
	{
		Assert(buttonId <= MOUSE_BUTTON_COUNT);
		m_mouseButtons[buttonId] = state;
	}

}