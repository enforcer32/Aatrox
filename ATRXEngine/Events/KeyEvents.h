#pragma once

#include "ATRXEngine/Events/Event.h"
#include "ATRXEngine/Input/InputCodes.h"

namespace ATRX
{
	class KeyPressEvent : public Event
	{
	public:
		KeyPressEvent(KeyCode key)
			: m_Key(key) {}

		inline KeyCode GetKey() const { return m_Key; }

	private:
		KeyCode m_Key;
	};

	class KeyReleaseEvent : public Event
	{
	public:
		KeyReleaseEvent(KeyCode key)
			: m_Key(key) {}

		inline KeyCode GetKey() const { return m_Key; }

	private:
		KeyCode m_Key;
	};
}
