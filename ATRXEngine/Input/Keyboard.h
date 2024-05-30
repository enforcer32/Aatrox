#pragma once

#include "ATRXEngine/Input/InputCodes.h"

#define MAX_KEYBOARD_KEYS 348

namespace ATRX
{
	enum class KeyEventType
	{
		Invalid,
		Press,
		Release,
		Repeat,
	};

	class Keyboard
	{
	public:
		bool OnInit();
		void OnDestroy();
		void OnUpdate();
		void Reset();

		bool IsKeyUp(KeyCode key) const;
		bool IsKeyDown(KeyCode key) const;
		bool WasKeyUp(KeyCode key) const;
		bool WasKeyDown(KeyCode key) const;

		void OnKeyEvent(KeyEventType type, KeyCode key);

	private:
		bool m_Initialized = false;
		bool m_KeyState[MAX_KEYBOARD_KEYS + 1];
		bool m_OldKeyState[MAX_KEYBOARD_KEYS + 1];
	};
}
