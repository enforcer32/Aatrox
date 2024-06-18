#include "ATRXEngine/Platform/RHI/Vulkan/VulkanDevice.h"
#include "ATRXEngine/Core/Logger.h"

namespace ATRX
{
	bool VulkanDevice::OnInit(const std::shared_ptr<RendererContext>& context)
	{
		ATRX_LOG_INFO("ATRXVulkanDevice->Initializing VulkanDevice...");
		m_Context = std::dynamic_pointer_cast<VulkanContext>(context);
		
		m_PhysicalDevice = std::make_shared<VulkanPhysicalDevice>();
		if (!m_PhysicalDevice->OnInit(m_Context))
		{
			ATRX_LOG_ERROR("ATRXVulkanDevice->Error Initializing VulkanPhysicalDevice!");
			return false;
		}

		if (!InitLogicalDevice())
		{
			ATRX_LOG_ERROR("ATRXVulkanDevice->Error InitLogicalDevice!");
			return false;
		}

		SetupQueueHandles();

		ATRX_LOG_INFO("ATRXVulkanDevice->Initialized VulkanDevice!");
		return m_Initialized = true;
	}

	void VulkanDevice::OnDestroy()
	{
		if (m_Initialized)
		{
			ATRX_LOG_INFO("ATRXVulkanDevice->Destroying...");
			vkDestroyDevice(m_LogicalDevice, m_Context->GetAllocator());
			m_PhysicalDevice->OnDestroy();
			m_Context = nullptr;
			ATRX_LOG_INFO("ATRXVulkanDevice->Destroyed!");
			m_Initialized = false;
		}
	}

	VkDevice VulkanDevice::GetInternalDevice() const
	{
		return m_LogicalDevice;
	}

	const std::shared_ptr<VulkanPhysicalDevice>& VulkanDevice::GetPhysicalDevice() const
	{
		return m_PhysicalDevice;
	}

	const char* VulkanDevice::GetDeviceName() const
	{
		return m_PhysicalDevice->GetDeviceName();
	}

	VkQueue VulkanDevice::GetGraphicsQueue() const
	{
		return m_GraphicsQueue;
	}

	VkQueue VulkanDevice::GetComputeQueue() const
	{
		return m_ComputeQueue;
	}

	VkQueue VulkanDevice::GetTransferQueue() const
	{
		return m_TransferQueue;
	}

	bool VulkanDevice::InitLogicalDevice()
	{
		ATRX_LOG_INFO("ATRXVulkanDevice->Initializing LogicalDevice...");
		std::vector<const char*> deviceExtensions;
		deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		VkDeviceCreateInfo deviceCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
		deviceCreateInfo.queueCreateInfoCount = m_PhysicalDevice->GetRequestedQueueCreateInfos().size();
		deviceCreateInfo.pQueueCreateInfos = m_PhysicalDevice->GetRequestedQueueCreateInfos().data();
		deviceCreateInfo.pEnabledFeatures = &m_PhysicalDevice->GetRequestedFeatures();
		deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

		VkResult res = vkCreateDevice(m_PhysicalDevice->GetInternalPhysicalDevice(), &deviceCreateInfo, m_Context->GetAllocator(), &m_LogicalDevice);
		if (res != VK_SUCCESS)
		{
			ATRX_LOG_ERROR("ATRXVulkanDevice->Error vkCreateDevice: ({})!", (int)res);
			return false;
		}

		ATRX_LOG_INFO("ATRXVulkanDevice->Initialized LogicalDevice!");
		return true;
	}

	void VulkanDevice::SetupQueueHandles()
	{
		vkGetDeviceQueue(m_LogicalDevice, m_PhysicalDevice->GetQueueFamilyIndices().Graphics, 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_LogicalDevice, m_PhysicalDevice->GetQueueFamilyIndices().Compute, 0, &m_ComputeQueue);
		vkGetDeviceQueue(m_LogicalDevice, m_PhysicalDevice->GetQueueFamilyIndices().Transfer, 0, &m_TransferQueue);
	}
}
