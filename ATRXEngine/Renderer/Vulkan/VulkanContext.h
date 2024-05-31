#pragma once

#include <vulkan/vulkan.h>

namespace ATRX
{
	class VulkanContext
	{
	public:
		bool OnInit();
		void OnDestroy();

	private:
		bool m_Initialized = false;
		VkInstance m_Instance;
		VkAllocationCallbacks* m_Allocator = nullptr;
	};
}
