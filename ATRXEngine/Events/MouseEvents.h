#pragma once

#include "ATRXEngine/Events/Event.h"
#include "ATRXEngine/Input/InputCodes.h"

namespace ATRX
{
	class MouseButtonPressEvent : public Event
	{
	public:
		MouseButtonPressEvent(MouseCode button)
			: m_Button(button) {}

		inline MouseCode GetButton() const { return m_Button; }

	private:
		MouseCode m_Button;
	};

	class MouseButtonReleaseEvent : public Event
	{
	public:
		MouseButtonReleaseEvent(MouseCode button)
			: m_Button(button) {}

		inline MouseCode GetButton() const { return m_Button; }

	private:
		MouseCode m_Button;
	};

	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(int32_t state)
			: m_State(state) {}

		inline int32_t GetState() const { return m_State; }

	private:
		int32_t m_State;
	};

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(MousePoint<double> position)
			: m_Position(position) {}

		inline MousePoint<double> GetPosition() const { return m_Position; }

	private:
		MousePoint<double> m_Position;
	};
}
