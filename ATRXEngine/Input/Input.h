#pragma once

#include "ATRXEngine/Core/Typedef.h"
#include "ATRXEngine/Input/Keyboard.h"
#include "ATRXEngine/Input/Mouse.h"

namespace ATRX
{
	class ATRX_API Input
	{
	public:
		// Keyboard
		static bool IsKeyUp(KeyCode key);
		static bool IsKeyDown(KeyCode key);
		static bool WasKeyUp(KeyCode key);
		static bool WasKeyDown(KeyCode key);

		// Mouse
		static bool IsMouseButtonUp(MouseCode button);
		static bool IsMouseButtonDown(MouseCode button);
		static bool WasMouseButtonUp(MouseCode button);
		static bool WasMouseButtonDown(MouseCode button);
		static MousePoint<double> GetPosition();
		static MousePoint<double> GetOldPosition();

		// Processors
		static void OnKeyEvent(KeyEventType type, KeyCode key);
		static void OnMouseButtonEvent(MouseEventType type, MouseCode button);
		static void OnMouseScrollEvent(int32_t state);
		static void OnMouseMoveEvent(MousePoint<double> position);

	private:
		friend class Engine;
		static bool OnInit();
		static void OnDestroy();
		static void OnUpdate();
		static void Reset();

	private:
		static bool m_Initialized;
		static Keyboard m_Keyboard;
		static Mouse m_Mouse;
	};
}
