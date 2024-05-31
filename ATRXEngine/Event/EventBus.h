#pragma once

#include "ATRXEngine/Event/EventCallback.h"
#include "ATRXEngine/Core/Logger.h"

#include <map>
#include <typeindex>
#include <memory>

namespace ATRX
{
	class EventBus
	{
	public:
		static bool OnInit();
		static void OnDestroy();

		template<typename TEvent, typename TOwner>
		static void Subscribe(TOwner* owner, void(TOwner::*callbackFunction)(TEvent&))
		{
			std::shared_ptr<IEventCallback> subscriber = std::make_shared<EventCallback<TEvent, TOwner>>(owner, callbackFunction);
			m_Subscribers[typeid(TEvent)].push_back(subscriber);
			ATRX_LOG_DEBUG("ATRXEventBus->Subscribed({} to Event {})!", typeid(TOwner).name(), typeid(TEvent).name());
		}

		template<typename TEvent, typename TOwner>
		static void Unsubscribe(TOwner* owner)
		{
			auto& subscribers = m_Subscribers[typeid(TEvent)];
			for (auto subscriber : subscribers)
			{
				std::shared_ptr<EventCallback<TEvent, TOwner>> sub = std::dynamic_pointer_cast<EventCallback<TEvent, TOwner>>(subscriber);
				if (sub->GetOwner() == owner)
				{
					m_Subscribers[typeid(TEvent)].remove(sub);
					ATRX_LOG_DEBUG("ATRXEventBus->Unsubscribed({} from Event {})!", typeid(TOwner).name(), typeid(TEvent).name());
					break;
				}
			}
		}

		template<typename TEvent, typename ...Args>
		static void Emit(Args&& ...args)
		{
			auto& handlers = m_Subscribers[typeid(TEvent)];
			for (auto& handler : handlers)
			{
				TEvent ev(std::forward<Args>(args)...);
				handler->Handle(ev);
			}
		}

	private:
		static bool m_Initialized;
		static std::map<std::type_index, std::list<std::shared_ptr<IEventCallback>>> m_Subscribers;
	};
}
