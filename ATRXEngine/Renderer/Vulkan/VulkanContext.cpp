#include "ATRXEngine/Renderer/Vulkan/VulkanContext.h"
#include "ATRXEngine/Core/Logger.h"

namespace ATRX
{
	bool VulkanContext::OnInit()
	{
		VkApplicationInfo appInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
		appInfo.apiVersion = VK_API_VERSION_1_2;
		appInfo.pApplicationName = "Aatrox";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Aatrox";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		
		VkInstanceCreateInfo instanceInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
		instanceInfo.pApplicationInfo = &appInfo;
		instanceInfo.enabledExtensionCount = 0;
		instanceInfo.ppEnabledExtensionNames = 0;
		instanceInfo.enabledLayerCount = 0;
		instanceInfo.ppEnabledLayerNames = 0;

		VkResult res = vkCreateInstance(&instanceInfo, m_Allocator, &m_Instance);
		if (res != VK_SUCCESS)
		{
			ATRX_LOG_ERROR("VulkanContext->vkCreateInstance Failed: {}", (int)res);
			return false;
		}

		return true;
	}

	void VulkanContext::OnDestroy()
	{
		if (m_Initialized)
		{
			m_Initialized = false;
		}
	}
}
