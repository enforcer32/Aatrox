#include "ATRXEngine/Input/Input.h"

namespace ATRX
{
	bool Input::m_Initialized = false;

	bool Input::OnInit()
	{
		if (!m_Keyboard.Init()) return false;
		if (!m_Mouse.Init()) return false;
		return m_Initialized = true;
	}

	void Input::OnDestroy()
	{
		if (m_Initialized)
		{
			m_Mouse.Destroy();
			m_Keyboard.Destroy();
			m_Initialized = false;
		}
	}

	void Input::OnUpdate()
	{
		m_Keyboard.OnUpdate();
		m_Mouse.OnUpdate();
	}

	void Input::Reset()
	{
		m_Keyboard.Reset();
		m_Mouse.Reset();
	}

	bool Input::IsKeyUp(KeyCode key)
	{
		return m_Keyboard.IsKeyUp(key);
	}

	bool Input::IsKeyDown(KeyCode key)
	{
		return m_Keyboard.IsKeyDown(key);
	}

	bool Input::WasKeyUp(KeyCode key)
	{
		return m_Keyboard.WasKeyUp(key);
	}

	bool Input::WasKeyDown(KeyCode key)
	{
		return m_Keyboard.WasKeyDown(key);
	}

	bool Input::IsMouseButtonUp(MouseCode button)
	{
		return m_Mouse.IsButtonUp(button);
	}

	bool Input::IsMouseButtonDown(MouseCode button)
	{
		return m_Mouse.IsButtonDown(button);
	}

	bool Input::WasMouseButtonUp(MouseCode button)
	{
		return m_Mouse.WasButtonUp(button);
	}

	bool Input::WasMouseButtonDown(MouseCode button)
	{
		return m_Mouse.WasButtonDown(button);
	}

	MousePoint<double> Input::GetPosition()
	{
		return m_Mouse.GetPosition();
	}

	MousePoint<double> Input::GetOldPosition()
	{
		return m_Mouse.GetOldPosition();
	}

	void Input::OnKeyEvent(KeyEventType type, KeyCode key)
	{
		m_Keyboard.OnKeyEvent(type, key);
	}

	void Input::OnMouseButtonEvent(MouseEventType type, MouseCode button)
	{
		m_Mouse.OnMouseButtonEvent(type, button);
	}

	void Input::OnMouseScrollEvent(int32_t state)
	{
		m_Mouse.OnMouseScrollEvent(state);
	}

	void Input::OnMouseMoveEvent(MousePoint<double> position)
	{
		m_Mouse.OnMouseMoveEvent(position);
	}
}
