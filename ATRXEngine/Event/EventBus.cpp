#include "ATRXEngine/Event/EventBus.h"

namespace ATRX
{
	bool EventBus::m_Initialized = true;
	std::map<std::type_index, std::list<std::shared_ptr<IEventCallback>>> EventBus::m_Subscribers;
	
	bool EventBus::OnInit()
	{
		return m_Initialized = true;
	}

	void EventBus::OnDestroy()
	{
		if (m_Initialized)
		{
			m_Subscribers.clear();
			m_Initialized = false;
		}
	}
}
