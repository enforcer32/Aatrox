#pragma once

#include "ATRXEngine/Events/Event.h"

#include <functional>

namespace ATRX
{
	class IEventCallback
	{
	public:
		virtual ~IEventCallback() = default;

		void Handle(Event& ev)
		{
			Invoker(ev);
		}

	private:
		virtual void Invoker(Event& ev) = 0;
	};

	template<typename TEvent, typename TOwner>
	class EventCallback : public IEventCallback
	{
	private:
		typedef void(TOwner::* CallbackFunction)(TEvent&);

	public:
		EventCallback(TOwner* owner, CallbackFunction callback)
			: m_Owner(owner), m_Callback(callback)
		{
		}
		~EventCallback() = default;

		TOwner* GetOwner()
		{
			return m_Owner;
		}

	private:
		virtual void Invoker(Event& ev) override
		{
			std::invoke(m_Callback, m_Owner, (TEvent&)ev);
		}

	private:
		TOwner* m_Owner;
		CallbackFunction m_Callback;
	};
}
