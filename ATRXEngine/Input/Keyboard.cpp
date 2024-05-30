#include "ATRXEngine/Input/Keyboard.h"
#include <cstring>

namespace ATRX
{
	bool Keyboard::OnInit()
	{
		Reset();
		return m_Initialized = true;
	}

	void Keyboard::OnDestroy()
	{
		m_Initialized = false;
	}

	void Keyboard::OnUpdate()
	{
		std::memcpy(m_OldKeyState, m_KeyState, sizeof(m_OldKeyState));
	}

	void Keyboard::Reset()
	{
		std::memset(m_KeyState, 0, sizeof(m_KeyState));
		std::memset(m_OldKeyState, 0, sizeof(m_OldKeyState));
	}

	bool Keyboard::IsKeyUp(KeyCode key) const
	{
		return !m_KeyState[key];
	}

	bool Keyboard::IsKeyDown(KeyCode key) const
	{
		return m_KeyState[key];
	}

	bool Keyboard::WasKeyUp(KeyCode key) const
	{
		return !m_OldKeyState[key];
	}

	bool Keyboard::WasKeyDown(KeyCode key) const
	{
		return m_OldKeyState[key];
	}

	void Keyboard::OnKeyEvent(KeyEventType type, KeyCode key)
	{
		switch (type)
		{
		case KeyEventType::Press:
			m_KeyState[key] = 1;
			break;
		case KeyEventType::Release:
			m_KeyState[key] = 0;
			break;
		default:
			break;
		}
	}
}
