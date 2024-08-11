#ifndef INPUTMANAGERWIN32_H
#define INPUTMANAGERWIN32_H

#include "engine/inputmanager.h"

namespace solunar
{
	class InputManager_Win32 : public InputManager
	{
		//IMPLEMENT_OBJECT(InputManager_Win32, InputManager);
	private:
		static InputManager_Win32 ms_inputManager;

	public:
		InputManager_Win32();
		~InputManager_Win32();

		// Update the input manager
		void Update() override;

		// keyboard action
		void KeyboardAction(uint32_t keyId, bool state);

	private:
		bool m_lastCursorHiding;
	};
}

#endif // !INPUTMANAGERWIN32_H
