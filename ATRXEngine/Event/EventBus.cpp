#include "ATRXEngine/Event/EventBus.h"

namespace ATRX
{
	bool EventBus::m_Initialized = true;
	std::map<std::type_index, std::list<std::shared_ptr<IEventCallback>>> EventBus::m_Subscribers;
	
	bool EventBus::OnInit()
	{
		ATRX_LOG_INFO("ATRXEventBus->Initialized!");
		return m_Initialized = true;
	}

	void EventBus::OnDestroy()
	{
		if (m_Initialized)
		{
			m_Subscribers.clear();
			ATRX_LOG_INFO("ATRXEventBus->Destroyed!");
			m_Initialized = false;
		}
	}
}
