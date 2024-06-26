#pragma once

#include <cstdint>

namespace ATRX
{
	enum KeyCode : uint16_t
	{
		/* Keys */
		Unknown = 0,
		Space = 0x20,
		Apostrophe = 0xDE,
		Comma = 0xBC,
		Minus = 0xBD,
		Period = 0xBE,
		Slash = 0xBF,

		/* Numeric Keys */
		N0 = 0x30,
		N1 = 0x31,
		N2 = 0x32,
		N3 = 0x33,
		N4 = 0x34,
		N5 = 0x35,
		N6 = 0x36,
		N7 = 0x37,
		N8 = 0x38,
		N9 = 0x39,

		/* Keys */
		Semicolon = 0x3B,
		Equal = 0xBB,

		/* Letters */
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

		/* Keys */
		LeftBracket = 0xDB,
		Backslash = 0xDC,
		RightBracket = 0xDD,
		GraveAccent = 0xC0,

		/* non-US #1 and #2 */
		World1 = 161,
		World2 = 162,

		/* Function keys */
		Escape = 0x1B,
		Enter = 0x0D,
		Tab = 0x09,
		Backspace = 0x08,
		Insert = 0x2D,
		Delete = 0x2E,
		Right = 0x27,
		Left = 0x25,
		Down = 0x28,
		Up = 0x26,
		PageUp = 0x21,
		PageDown = 0x22,
		Home = 0x24,
		End = 0x23,
		CapsLock = 0x14,
		ScrollLock = 0x91,
		NumLock = 0x90,
		PrintScreen = 0x2C,
		Pause = 0x13,
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
		F25 = 0x88,

		/* Keypad */
		KP0 = 0x60,
		KP1 = 0x61,
		KP2 = 0x62,
		KP3 = 0x63,
		KP4 = 0x64,
		KP5 = 0x65,
		KP6 = 0x66,
		KP7 = 0x67,
		KP8 = 0x69,
		KP9 = 0x69,
		KPDecimal = 0x6E,
		KPDivide = 0x6F,
		KPMultiply = 0x6A,
		KPSubtract = 0x6D,
		KPAdd = 0x6B,
		KPEnter = 335,
		KPEqual = 0x92,

		/* Keys */
		LeftShift = 0xA0,
		LeftControl = 0xA2,
		LeftAlt = 0xA4,
		LeftSuper = 0x5B,
		RightShift = 0xA1,
		RightControl = 0xA3,
		RightAlt = 0xA5,
		RightSuper = 0x5C,
		Menu = 348,
	};

	enum MouseCode : uint16_t
	{
		Button1 = 0,
		Button2 = 1,
		Button3 = 2,
		Button4 = 3,
		Button5 = 4,
		Button6 = 5,
		Button7 = 6,
		Button8 = 7,

		ButtonLast = Button8,
		ButtonLeft = Button1,
		ButtonRight = Button2,
		ButtonMiddle = Button3,
	};

	struct MousePosition
	{
		double XPos;
		double YPos;
	};
}
