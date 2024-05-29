#include "ATRXEngine/Core/Event.h"

namespace ATRX
{
	bool EventManager::OnInit()
	{
		return m_Initialized = true;
	}

	void EventManager::OnDestroy()
	{
		if (m_Initialized)
		{
			m_Subscribers.clear();
			m_Initialized = false;
		}
	}
}
