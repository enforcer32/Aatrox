#include "ATRXEngine/Input/Mouse.h"
#include "ATRXEngine/Core/Logger.h"

#include <cstring>

namespace ATRX
{
	bool Mouse::OnInit()
	{
		ATRX_LOG_INFO("ATRXMouse->Initializing...");
		std::memset(m_ButtonState, 0, sizeof(m_ButtonState));
		m_ScrollState = 0;
		std::memset(&m_Position, 0, sizeof(m_Position));
		ATRX_LOG_INFO("ATRXMouse->Initialized!");
		return m_Initialized = true;
	}

	void Mouse::OnDestroy()
	{
		ATRX_LOG_INFO("ATRXMouse->Destroying...");
		ATRX_LOG_INFO("ATRXMouse->Destroyed!");
		m_Initialized = false;
	}

	void Mouse::OnUpdate()
	{
		std::memcpy(m_OldButtonState, m_ButtonState, sizeof(m_OldButtonState));
		m_OldScrollState = m_ScrollState;
		m_OldPosition = m_Position;
	}

	void Mouse::Reset()
	{
		std::memset(m_ButtonState, 0, sizeof(m_ButtonState));
		m_ScrollState = 0;
		std::memset(&m_Position, 0, sizeof(m_Position));
	}

	bool Mouse::IsButtonUp(MouseCode button) const
	{
		return !m_ButtonState[button];
	}

	bool Mouse::IsButtonDown(MouseCode button) const
	{
		return m_ButtonState[button];
	}

	bool Mouse::WasButtonUp(MouseCode button) const
	{
		return !m_OldButtonState[button];
	}

	bool Mouse::WasButtonDown(MouseCode button) const
	{
		return m_OldButtonState[button];
	}

	MousePoint<double> Mouse::GetPosition()
	{
		return m_Position;
	}

	MousePoint<double> Mouse::GetOldPosition()
	{
		return m_OldPosition;
	}
	
	void Mouse::OnMouseButtonEvent(MouseEventType type, MouseCode button)
	{
		switch (type)
		{
		case ATRX::MouseEventType::Press:
			m_ButtonState[button] = true;
			break;
		case ATRX::MouseEventType::Release:
			m_ButtonState[button] = false;
			break;
		default:
			break;
		}
	}

	void Mouse::OnMouseScrollEvent(int32_t state)
	{
		m_ScrollState = state;
	}

	void Mouse::OnMouseMoveEvent(MousePoint<double> position)
	{
		m_Position = position;
	}
}
