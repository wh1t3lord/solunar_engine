#ifndef INPUTMANAGERWIN32_H
#define INPUTMANAGERWIN32_H

#include "engine/inputmanager.h"

namespace engine
{
	class InputManager_Win32 : public InputManager
	{
		//ImplementObject(InputManager_Win32, InputManager);
	private:
		static InputManager_Win32 ms_inputManager;

	public:
		InputManager_Win32();
		~InputManager_Win32();

		// Update the input manager
		void update() override;

		// keyboard action
		void keyboardAction(uint32_t keyId, bool state);

	private:
		bool m_lastCursorHiding;
	};
}

#endif // !INPUTMANAGERWIN32_H
