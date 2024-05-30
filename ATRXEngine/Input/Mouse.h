#pragma once

#include "ATRXEngine/Input/InputCodes.h"

#define MAX_MOUSE_BUTTONS 8

namespace ATRX
{
	template<typename T>
	struct MousePoint
	{
		T x, y;

		MousePoint() : x(0), y(0) {}
		MousePoint(T x, T y) : x(x), y(y) {}
	};

	enum class MouseEventType
	{
		Invalid,
		Press,
		Release,
		ScrollUp,
		ScrollDown,
		Move,
	};

	class Mouse
	{
	public:
		bool OnInit();
		void OnDestroy();
		void OnUpdate();
		void Reset();

		bool IsButtonUp(MouseCode button) const;
		bool IsButtonDown(MouseCode button) const;
		bool WasButtonUp(MouseCode button) const;
		bool WasButtonDown(MouseCode button) const;
		MousePoint<double> GetPosition();
		MousePoint<double> GetOldPosition();

		void OnMouseButtonEvent(MouseEventType type, MouseCode button);
		void OnMouseScrollEvent(int32_t state);
		void OnMouseMoveEvent(MousePoint<double> position);

	private:
		bool m_Initialized = true;
		bool m_ButtonState[MAX_MOUSE_BUTTONS];
		bool m_OldButtonState[MAX_MOUSE_BUTTONS];
		int32_t m_ScrollState;
		int32_t m_OldScrollState;
		MousePoint<double> m_Position;
		MousePoint<double> m_OldPosition;
	};
}
