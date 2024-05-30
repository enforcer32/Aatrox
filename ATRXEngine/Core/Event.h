#pragma once

#include "ATRXEngine/Events/Event.h"

#include <map>
#include <typeindex>
#include <functional>
#include <memory>

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

	class EventManager
	{
	public:
		bool OnInit();
		void OnDestroy();

		template<typename TEvent, typename TOwner>
		void Subscribe(TOwner* owner, void(TOwner::*callbackFunction)(TEvent&))
		{
			std::shared_ptr<IEventCallback> subscriber = std::make_shared<EventCallback<TEvent, TOwner>>(owner, callbackFunction);
			m_Subscribers[typeid(TEvent)].push_back(subscriber);
		}

		template<typename TEvent, typename TOwner>
		void Unsubscribe(TOwner* owner)
		{
			auto& subscribers = m_Subscribers[typeid(TEvent)];
			for (auto subscriber : subscribers)
			{
				std::shared_ptr<EventCallback<TEvent, TOwner>> sub = std::dynamic_pointer_cast<EventCallback<TEvent, TOwner>>(subscriber);
				if (sub->GetOwner() == owner)
				{
					m_Subscribers[typeid(TEvent)].remove(sub);
					break;
				}
			}
		}

		template<typename TEvent, typename ...Args>
		void Emit(Args&& ...args)
		{
			auto& handlers = m_Subscribers[typeid(TEvent)];
			for (auto& handler : handlers)
			{
				TEvent ev(std::forward<Args>(args)...);
				handler->Handle(ev);
			}
		}

	private:
		bool m_Initialized = false;
		std::map<std::type_index, std::list<std::shared_ptr<IEventCallback>>> m_Subscribers;
	};
}