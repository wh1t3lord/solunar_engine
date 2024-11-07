#ifndef INPUTMANAGER_DINPUT_H
#define INPUTMANAGER_DINPUT_H

#include "engine/inputmanager.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace solunar
{
	class InputManager_DInput : public InputManager
	{
		//IMPLEMENT_OBJECT(InputManager_DInput, InputManager);
	public:
		InputManager_DInput();
		~InputManager_DInput();

		void Init(HWND hWindow);
		void Shutdown();

		// Update the input manager
		void Update() override;

		// keyboard action
		void KeyboardAction(uint32_t keyId, bool state);

		// mouse button action
		void MouseButtonAction(MouseButtons buttonId, bool state);

	private:
		void ReacquireDevices();

	private:
		IDirectInput8* m_pDirectInput;
		IDirectInputDevice8* m_pDIKeyboard;

		bool m_lastCursorHiding;
	};
}

#endif // !INPUTMANAGER_DINPUT_H
