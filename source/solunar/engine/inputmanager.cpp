#include "enginepch.h"
#include "engine/inputmanager.h"
#include "main/main.h"

namespace engine
{
	InputManager InputManager::ms_inputManager;

	InputManager::InputManager()
	{
		memset(m_keys, 0, sizeof(m_keys));
		m_cursorPos = glm::vec2(0.0f);
		m_deltaCursorPos = glm::vec2(0.0f);
		m_lastCurPos = glm::vec2(0.0f);
		m_firstMouse = true;
	}

	InputManager::~InputManager()
	{
	}

	void InputManager::update()
	{
		m_lastCodePoint = 0;

		// #TODO: CURSOR
		//if (GetForegroundWindow() == (HWND)appGetWindow())
		//{
			POINT pt = {};
			GetCursorPos(&pt);
			cursorPosAction(static_cast<float>(pt.x), static_cast<float>(pt.y));
		//}
	}

	void InputManager::keyboardAction(uint32_t keyId, bool state)
	{
		if (keyId >= 420)
			return;

		m_keys[keyId] = state;
	}

	void InputManager::cursorPosAction(float x, float y)
	{
		setCursorPosCallback(true);
		if (m_firstMouse)
		{
			m_lastCurPos.x = x;
			m_lastCurPos.y = y;
			m_firstMouse = false;
		}

		m_deltaCursorPos.x = x - m_lastCurPos.x;
		m_deltaCursorPos.y = m_lastCurPos.y - y;

		m_lastCurPos.x = x;
		m_lastCurPos.y = y;

		m_cursorPos = m_lastCurPos;
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

}