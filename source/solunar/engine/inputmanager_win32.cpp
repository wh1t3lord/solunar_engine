#include "enginepch.h"
#include "main/main.h"
#include "engine/inputmanager_win32.h"

namespace engine
{
	InputManager_Win32 InputManager_Win32::ms_inputManager;

	InputManager_Win32::InputManager_Win32() :
		m_lastCursorHiding(false)
	{
	}

	InputManager_Win32::~InputManager_Win32()
	{
	}

	void InputManager_Win32::update()
	{
		InputManager::update();

		if (m_lastCursorHiding != m_hideCursor)
		{
			if (m_hideCursor)
				ShowCursor(FALSE);
			else
				ShowCursor(TRUE);

			m_lastCursorHiding = m_hideCursor;
		}

		if (m_captureCursor)
		{
			//ShowCursor(FALSE);

			RECT rect = {};
			GetClientRect((HWND)appGetWindow(), &rect);

			int width = rect.right - rect.left;
			int height = rect.bottom - rect.top;

			int halfWidth = width / 2;
			int halfHeight = height / 2;

			POINT pt = {};
			GetCursorPos(&pt);
			
			SetCursorPos(halfWidth, halfHeight);

			pt.x = pt.x - halfWidth;
			pt.y = pt.y - halfHeight;

			m_cursorPos.x = static_cast<float>(pt.x);
			m_cursorPos.y = static_cast<float>(pt.y);

#if 0
			m_deltaCursorPos.x = static_cast<float>(pt.x);
			m_deltaCursorPos.y = static_cast<float>(-pt.y);
			
#else
			m_deltaCursorPos.x += m_cursorPos.x - m_lastCursorPos.x;
			m_deltaCursorPos.y += m_lastCursorPos.y - m_cursorPos.y;
#endif
			//cursorPosAction(static_cast<float>(pt.x), static_cast<float>(pt.y));

			m_lastCursorPos = m_cursorPos;

#if 0
			char buf[256];
			snprintf(buf, sizeof(buf), "%i %i", pt.x, pt.y);
			g_fontManager->drawSystemFontShadowed(buf, 300, 300, glm::vec4(1.0f, 1.0f,1.0f, 1.0f));
#endif
		}
		else
		{
			resetDelta();
		//	ShowCursor(TRUE);
		}
	}

	void InputManager_Win32::keyboardAction(uint32_t keyId, bool state)
	{
		if (keyId >= 420)
			return;

		m_keys[keyId] = state;
	}

#if 0
	void InputManager_Win32::cursorPosAction(float x, float y)
	{
	/*	m_lastCursorPos.x = x;
		m_lastCursorPos.y = y;

		m_deltaCursorPos.x = x - m_lastCursorPos.x;
		m_deltaCursorPos.y = m_lastCursorPos.y - y;

		m_lastCursorPos.x = x;
		m_lastCursorPos.y = y;

		m_cursorPos = m_lastCursorPos;*/
	}
#endif

}