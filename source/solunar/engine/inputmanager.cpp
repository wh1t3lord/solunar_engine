#include "enginepch.h"
#include "engine/inputmanager.h"

namespace engine
{

InputManager::InputManager() :
	m_cursorPos(0.0f),
	m_lastCursorPos(0.0f),
	m_deltaCursorPos(0.0f),
	m_captureCursor(false),
	m_hideCursor(false)
{
	memset(m_keys, 0, sizeof(m_keys));
}

InputManager::~InputManager()
{
}

void InputManager::update()
{
}

bool InputManager::isPressed(uint32_t key)
{
	Assert(key <= KEY_COUNT);
	return m_keys[key];
}

bool InputManager::isPressedWithReset(uint32_t key)
{
	Assert(key <= KEY_COUNT);
	bool value = m_keys[key];
	m_keys[key] = false;
	return value;
}

void InputManager::setCursorCapture(bool capture)
{
	m_captureCursor = capture;
}

void InputManager::setCursorHiding(bool hide)
{
	m_hideCursor = hide;
}

void InputManager::resetDelta()
{
	m_deltaCursorPos.x = 0.0f;
	m_deltaCursorPos.y = 0.0f;
}

}