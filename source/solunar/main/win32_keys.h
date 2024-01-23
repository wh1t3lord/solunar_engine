#ifndef WIN32_KEYS_H
#define WIN32_KEYS_H

#include "engine/inputmanager.h"

namespace Win32Keys
{
	enum Keys
	{
		Backspace = 0x08,
		Tab = 0x09,
		Clear = 0x0C,
		Enter = 0x0D,
		Shift = 0x10,
		Control = 0x11,
		Alt = 0x12,
		Pause = 0x13,
		CapsLock = 0x14,
		Escape = 0x1B,
		Space = 0x20,
		PageUp = 0x21,
		PageDown = 0x22,
		End = 0x23,
		Home = 0x24,
		Left = 0x25,
		Up = 0x26,
		Right = 0x27,
		Down = 0x28,
		Select = 0x29,
		Print = 0x2A,
		Execute = 0x2B,
		PrintScreen = 0x2C,
		Insert = 0x2D,
		Delete = 0x2E,
		Help = 0x2F,
		Zero = 0x30,
		One = 0x31,
		Two = 0x32,
		Three = 0x33,
		Four = 0x34,
		Five = 0x35,
		Six = 0x36,
		Seven = 0x37,
		Eight = 0x38,
		Nine = 0x39,
		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,
		LeftWindowsKey = 0x5B,
		RightWindowsKey = 0x5C,
		ApplicationsKey = 0x5D,
		Sleep = 0x5F,
		NumPad0 = 0x60,
		NumPad1 = 0x61,
		NumPad2 = 0x62,
		NumPad3 = 0x63,
		NumPad4 = 0x64,
		NumPad5 = 0x65,
		NumPad6 = 0x66,
		NumPad7 = 0x67,
		NumPad8 = 0x68,
		NumPad9 = 0x69,
		Multiply = 0x6A,
		Add = 0x6B,
		Seperator = 0x6C,
		Subtract = 0x6D,
		Decimal = 0x6E,
		Divide = 0x6F,
		F1 = 0x70,
		F2 = 0x71,
		F3 = 0x72,
		F4 = 0x73,
		F5 = 0x74,
		F6 = 0x75,
		F7 = 0x76,
		F8 = 0x77,
		F9 = 0x78,
		F10 = 0x79,
		F11 = 0x7A,
		F12 = 0x7B,
		F13 = 0x7C,
		F14 = 0x7D,
		F15 = 0x7E,
		F16 = 0x7F,
		F17 = 0x80,
		F18 = 0x81,
		F19 = 0x82,
		F20 = 0x83,
		F21 = 0x84,
		F22 = 0x85,
		F23 = 0x86,
		F24 = 0x87,
		Numlock = 0x90,
		ScrollLock = 0x91,
		LeftShift = 0xA0,
		RightShift = 0xA1,
		LeftControl = 0xA2,
		RightContol = 0xA3,
		LeftMenu = 0xA4,
		RightMenu = 0xA5,
		BrowserBack = 0xA6,
		BrowserForward = 0xA7,
		BrowserRefresh = 0xA8,
		BrowserStop = 0xA9,
		BrowserSearch = 0xAA,
		BrowserFavorites = 0xAB,
		BrowserHome = 0xAC,
		VolumeMute = 0xAD,
		VolumeDown = 0xAE,
		VolumeUp = 0xAF,
		NextTrack = 0xB0,
		PreviousTrack = 0xB1,
		StopMedia = 0xB2,
		PlayPause = 0xB3,
		LaunchMail = 0xB4,
		SelectMedia = 0xB5,
		LaunchApp1 = 0xB6,
		LaunchApp2 = 0xB7,
		OEM1 = 0xBA,
		OEMPlus = 0xB8,
		OEMComma = 0xBC,
		OEMMinus = 0xBD,
		OEMPeriod = 0xBE,
		OEM2 = 0xBF,
		OEM3 = 0xC0,
		OEM4 = 0xDB,
		OEM5 = 0xDC,
		OEM6 = 0xDD,
		OEM7 = 0xDE,
		OEM8 = 0xDF,
		OEM102 = 0xE2,
		Process = 0xE5,
		Packet = 0xE7,
		Attn = 0xF6,
		CrSel = 0xF7,
		ExSel = 0xF8,
		EraseEOF = 0xF9,
		Play = 0xFA,
		Zoom = 0xFB,
		PA1 = 0xFD,
		OEMClear = 0xFE
	};
}

namespace engine
{

	inline KeyboardKeys getKeyFromWin32(Win32Keys::Keys key)
	{
		switch (key)
		{
		case Win32Keys::Backspace:		return KeyboardKeys::KEY_BACKSPACE;
		case Win32Keys::Tab:			return KeyboardKeys::KEY_TAB;
		case Win32Keys::Clear:			return KeyboardKeys::KEY_WORLD_1;
		case Win32Keys::Enter:			return KeyboardKeys::KEY_ENTER;
		case Win32Keys::Shift:			return KeyboardKeys::KEY_UNKNOWN;//KEY_LEFT_SHIFT;		// TODO
		case Win32Keys::Control:		return KeyboardKeys::KEY_UNKNOWN;//KEY_LEFT_CONTROL;	// TODO
		case Win32Keys::Alt:			return KeyboardKeys::KEY_UNKNOWN;//KEY_LEFT_ALT;		// TODO
		case Win32Keys::Pause:			return KeyboardKeys::KEY_PAUSE;
		case Win32Keys::CapsLock:		return KeyboardKeys::KEY_CAPS_LOCK;
		case Win32Keys::Escape:			return KeyboardKeys::KEY_ESCAPE;
		case Win32Keys::Space:			return KeyboardKeys::KEY_SPACE;
		case Win32Keys::PageUp:			return KeyboardKeys::KEY_PAGE_UP;
		case Win32Keys::PageDown:		return KeyboardKeys::KEY_PAGE_DOWN;
		case Win32Keys::End:			return KeyboardKeys::KEY_END;
		case Win32Keys::Home:			return KeyboardKeys::KEY_HOME;
		case Win32Keys::Left:			return KeyboardKeys::KEY_LEFT;
		case Win32Keys::Up:				return KeyboardKeys::KEY_UP;
		case Win32Keys::Right:			return KeyboardKeys::KEY_RIGHT;
		case Win32Keys::Down:			return KeyboardKeys::KEY_DOWN;
		case Win32Keys::Select:			return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::Print:			return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::Execute:		return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::PrintScreen:	return KeyboardKeys::KEY_PRINT_SCREEN;
		case Win32Keys::Insert:			return KeyboardKeys::KEY_INSERT;
		case Win32Keys::Delete:			return KeyboardKeys::KEY_DELETE;
		case Win32Keys::Help:			return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::Zero:			return KeyboardKeys::KEY_0;
		case Win32Keys::One:			return KeyboardKeys::KEY_1;
		case Win32Keys::Two:			return KeyboardKeys::KEY_2;
		case Win32Keys::Three:			return KeyboardKeys::KEY_3;
		case Win32Keys::Four:			return KeyboardKeys::KEY_4;
		case Win32Keys::Five:			return KeyboardKeys::KEY_5;
		case Win32Keys::Six:			return KeyboardKeys::KEY_6;
		case Win32Keys::Seven:			return KeyboardKeys::KEY_7;
		case Win32Keys::Eight:			return KeyboardKeys::KEY_8;
		case Win32Keys::Nine:			return KeyboardKeys::KEY_9;
		case Win32Keys::A:				return KeyboardKeys::KEY_A;
		case Win32Keys::B:				return KeyboardKeys::KEY_B;
		case Win32Keys::C:				return KeyboardKeys::KEY_C;
		case Win32Keys::D:				return KeyboardKeys::KEY_D;
		case Win32Keys::E:				return KeyboardKeys::KEY_E;
		case Win32Keys::F:				return KeyboardKeys::KEY_F;
		case Win32Keys::G:				return KeyboardKeys::KEY_G;
		case Win32Keys::H:				return KeyboardKeys::KEY_H;
		case Win32Keys::I:				return KeyboardKeys::KEY_I;
		case Win32Keys::J:				return KeyboardKeys::KEY_J;
		case Win32Keys::K:				return KeyboardKeys::KEY_K;
		case Win32Keys::L:				return KeyboardKeys::KEY_L;
		case Win32Keys::M:				return KeyboardKeys::KEY_M;
		case Win32Keys::N:				return KeyboardKeys::KEY_N;
		case Win32Keys::O:				return KeyboardKeys::KEY_O;
		case Win32Keys::P:				return KeyboardKeys::KEY_P;
		case Win32Keys::Q:				return KeyboardKeys::KEY_Q;
		case Win32Keys::R:				return KeyboardKeys::KEY_R;
		case Win32Keys::S:				return KeyboardKeys::KEY_S;
		case Win32Keys::T:				return KeyboardKeys::KEY_T;
		case Win32Keys::U:				return KeyboardKeys::KEY_U;
		case Win32Keys::V:				return KeyboardKeys::KEY_V;
		case Win32Keys::W:				return KeyboardKeys::KEY_W;
		case Win32Keys::X:				return KeyboardKeys::KEY_X;
		case Win32Keys::Y:				return KeyboardKeys::KEY_Y;
		case Win32Keys::Z:				return KeyboardKeys::KEY_Z;
		case Win32Keys::LeftWindowsKey:	return KeyboardKeys::KEY_LEFT_SUPER;
		case Win32Keys::RightWindowsKey:return KeyboardKeys::KEY_RIGHT_SUPER;
		case Win32Keys::ApplicationsKey:return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::Sleep:			return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::NumPad0:		return KeyboardKeys::KEY_KP_0;
		case Win32Keys::NumPad1:		return KeyboardKeys::KEY_KP_1;
		case Win32Keys::NumPad2:		return KeyboardKeys::KEY_KP_2;
		case Win32Keys::NumPad3:		return KeyboardKeys::KEY_KP_3;
		case Win32Keys::NumPad4:		return KeyboardKeys::KEY_KP_4;
		case Win32Keys::NumPad5:		return KeyboardKeys::KEY_KP_5;
		case Win32Keys::NumPad6:		return KeyboardKeys::KEY_KP_6;
		case Win32Keys::NumPad7:		return KeyboardKeys::KEY_KP_7;
		case Win32Keys::NumPad8:		return KeyboardKeys::KEY_KP_8;
		case Win32Keys::NumPad9:		return KeyboardKeys::KEY_KP_9;
		case Win32Keys::Multiply:		return KeyboardKeys::KEY_KP_MULTIPLY;
		case Win32Keys::Add:			return KeyboardKeys::KEY_KP_ADD;
		case Win32Keys::Seperator:		return KeyboardKeys::KEY_KP_ENTER;
		case Win32Keys::Subtract:		return KeyboardKeys::KEY_KP_SUBTRACT;
		case Win32Keys::Decimal:		return KeyboardKeys::KEY_KP_DECIMAL;
		case Win32Keys::Divide:			return KeyboardKeys::KEY_KP_DIVIDE;
		case Win32Keys::F1:				return KeyboardKeys::KEY_F1;
		case Win32Keys::F2:				return KeyboardKeys::KEY_F2;
		case Win32Keys::F3:				return KeyboardKeys::KEY_F3;
		case Win32Keys::F4:				return KeyboardKeys::KEY_F4;
		case Win32Keys::F5:				return KeyboardKeys::KEY_F5;
		case Win32Keys::F6:				return KeyboardKeys::KEY_F6;
		case Win32Keys::F7:				return KeyboardKeys::KEY_F7;
		case Win32Keys::F8:				return KeyboardKeys::KEY_F8;
		case Win32Keys::F9:				return KeyboardKeys::KEY_F9;
		case Win32Keys::F10:			return KeyboardKeys::KEY_F10;
		case Win32Keys::F11:			return KeyboardKeys::KEY_F11;
		case Win32Keys::F12:			return KeyboardKeys::KEY_F12;
		case Win32Keys::F13:			return KeyboardKeys::KEY_F13;
		case Win32Keys::F14:			return KeyboardKeys::KEY_F14;
		case Win32Keys::F15:			return KeyboardKeys::KEY_F15;
		case Win32Keys::F16:			return KeyboardKeys::KEY_F16;
		case Win32Keys::F17:			return KeyboardKeys::KEY_F17;
		case Win32Keys::F18:			return KeyboardKeys::KEY_F18;
		case Win32Keys::F19:			return KeyboardKeys::KEY_F19;
		case Win32Keys::F20:			return KeyboardKeys::KEY_F20;
		case Win32Keys::F21:			return KeyboardKeys::KEY_F21;
		case Win32Keys::F22:			return KeyboardKeys::KEY_F22;
		case Win32Keys::F23:			return KeyboardKeys::KEY_F23;
		case Win32Keys::F24:			return KeyboardKeys::KEY_F24;
		case Win32Keys::Numlock:		return KeyboardKeys::KEY_NUM_LOCK;
		case Win32Keys::ScrollLock:		return KeyboardKeys::KEY_SCROLL_LOCK;
		case Win32Keys::LeftShift:		return KeyboardKeys::KEY_LEFT_SHIFT;
		case Win32Keys::RightShift:		return KeyboardKeys::KEY_RIGHT_SHIFT;
		case Win32Keys::LeftControl:	return KeyboardKeys::KEY_LEFT_CONTROL;
		case Win32Keys::RightContol:	return KeyboardKeys::KEY_RIGHT_CONTROL;
		case Win32Keys::LeftMenu:		return KeyboardKeys::KEY_MENU;
		case Win32Keys::RightMenu:		return KeyboardKeys::KEY_MENU;
		case Win32Keys::BrowserBack:	return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::BrowserForward:	return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::BrowserRefresh:	return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::BrowserStop:	return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::BrowserSearch:	return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::BrowserFavorites:return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::BrowserHome:return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::VolumeMute:return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::VolumeDown:return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::VolumeUp:return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::NextTrack:return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::PreviousTrack:return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::StopMedia:return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::PlayPause:return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::LaunchMail:return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::SelectMedia:return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::LaunchApp1:return KeyboardKeys::KEY_UNKNOWN; // TODO
		case Win32Keys::LaunchApp2:
			break;
		case Win32Keys::OEM1:
			break;
		case Win32Keys::OEMPlus:
			break;
		case Win32Keys::OEMComma:
			break;
		case Win32Keys::OEMMinus:
			break;
		case Win32Keys::OEMPeriod:
			break;
		case Win32Keys::OEM2:
			break;
		case Win32Keys::OEM3:
			break;
		case Win32Keys::OEM4:
			break;
		case Win32Keys::OEM5:
			break;
		case Win32Keys::OEM6:
			break;
		case Win32Keys::OEM7:
			break;
		case Win32Keys::OEM8:
			break;
		case Win32Keys::OEM102:
			break;
		case Win32Keys::Process:
			break;
		case Win32Keys::Packet:
			break;
		case Win32Keys::Attn:
			break;
		case Win32Keys::CrSel:
			break;
		case Win32Keys::ExSel:
			break;
		case Win32Keys::EraseEOF:
			break;
		case Win32Keys::Play:
			break;
		case Win32Keys::Zoom:
			break;
		case Win32Keys::PA1:
			break;
		case Win32Keys::OEMClear:
			break;
		default:
			break;
		}

		return KeyboardKeys::KEY_UNKNOWN;
	}

}

#endif // !WIN32_KEYS_H
