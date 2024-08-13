#include "enginepch.h"
#include "engine/inputmanager.h"

namespace solunar
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

void InputManager::Update()
{
}

bool InputManager::IsPressed(uint32_t key)
{
	Assert(key <= KEY_COUNT);
	return m_keys[key];
}

bool InputManager::IsPressedWithReset(uint32_t key)
{
	Assert(key <= KEY_COUNT);
	bool value = m_keys[key];
	m_keys[key] = false;
	return value;
}

bool InputManager::IsMouseButtonPressed(MouseButtons buttonId)
{
	Assert(buttonId <= MOUSE_BUTTON_COUNT);
	return m_mouseButtons[buttonId];
}

void InputManager::SetCursorCapture(bool capture)
{
	m_captureCursor = capture;
}

void InputManager::SetCursorHiding(bool hide)
{
	m_hideCursor = hide;
}

void InputManager::ResetDelta()
{
	m_deltaCursorPos.x = 0.0f;
	m_deltaCursorPos.y = 0.0f;
}

}