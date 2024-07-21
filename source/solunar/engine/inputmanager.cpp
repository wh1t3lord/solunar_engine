#include "enginepch.h"
#include "engine/inputmanager.h"
#include "main/main.h"

#include "graphics/ifontmanager.h"

namespace engine
{
	InputManager InputManager::ms_inputManager;

	InputManager::InputManager()
	{
		memset(m_keys, 0, sizeof(m_keys));
		m_cursorPos = glm::vec2(0.0f);
		m_lastCursorPos = glm::vec2(0.0f);
		m_deltaCursorPos = glm::vec2(0.0f);
	}

	InputManager::~InputManager()
	{
	}

	void InputManager::update()
	{
		m_lastCodePoint = 0;
		//m_deltaCursorPos = glm::vec2(0.0f);

		static bool toggleCursor = true;
		if (getKeyWithReset(KeyboardKeys::KEY_LEFT_ALT))
			toggleCursor = !toggleCursor;

		if (GetForegroundWindow() == (HWND)appGetWindow() && toggleCursor)
		{
			ShowCursor(FALSE);

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

			char buf[256];
			snprintf(buf, sizeof(buf), "%i %i", pt.x, pt.y);
			g_fontManager->drawSystemFontShadowed(buf, 300, 300, glm::vec4(1.0f, 1.0f,1.0f, 1.0f));
		}
		else
		{
			resetDelta();
			ShowCursor(TRUE);
		}
	}

	void InputManager::keyboardAction(uint32_t keyId, bool state)
	{
		if (keyId >= 420)
			return;

		m_keys[keyId] = state;
	}

	void InputManager::cursorPosAction(float x, float y)
	{
	/*	m_lastCursorPos.x = x;
		m_lastCursorPos.y = y;

		m_deltaCursorPos.x = x - m_lastCursorPos.x;
		m_deltaCursorPos.y = m_lastCursorPos.y - y;

		m_lastCursorPos.x = x;
		m_lastCursorPos.y = y;

		m_cursorPos = m_lastCursorPos;*/
	}

	void InputManager::characterAction(uint32_t codepoint)
	{
		m_lastCodePoint = codepoint;
	}

	bool InputManager::getKey(KeyboardKeys key)
	{
		if ((uint32_t)key >= 420)
			return false;

		return m_keys[(uint32_t)key];
	}

	bool InputManager::getKeyWithReset(KeyboardKeys key)
	{
		if ((uint32_t)key >= 420)
			return false;

		if (m_keys[(uint32_t)key])
		{
			m_keys[(uint32_t)key] = false;
			return true;
		}

		return false;
	}

	glm::vec2 InputManager::getDeltaCursorPos()
	{
		return m_deltaCursorPos;
	}

	void InputManager::resetDelta()
	{
		m_deltaCursorPos.x = 0.0f;
		m_deltaCursorPos.y = 0.0f;
	}

}