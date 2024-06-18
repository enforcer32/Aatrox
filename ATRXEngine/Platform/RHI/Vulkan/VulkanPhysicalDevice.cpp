#include "ATRXEngine/Platform/RHI/Vulkan/VulkanDevice.h"
#include "ATRXEngine/Core/Logger.h"

namespace ATRX
{
	bool VulkanPhysicalDevice::OnInit(const std::shared_ptr<VulkanContext>& context)
	{
		ATRX_LOG_INFO("ATRXVulkanPhysicalDevice->Initializing VulkanPhysicalDevice...");
		m_Context = context;

		if (!SelectDevice())
		{
			ATRX_LOG_ERROR("ATRXVulkanPhysicalDevice->Error Selecting Physical Device!");
			return false;
		}

		RequestDeviceQueues(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT);
		RequestDeviceFeatures();

		ATRX_LOG_INFO("ATRXVulkanPhysicalDevice->Initialized VulkanPhysicalDevice!");
		return m_Initialized = true;
	}

	void VulkanPhysicalDevice::OnDestroy()
	{
		if (m_Initialized)
		{
			ATRX_LOG_INFO("ATRXVulkanPhysicalDevice->Destroying...");
			m_PhysicalDevice = nullptr;
			m_Context = nullptr;
			ATRX_LOG_INFO("ATRXVulkanPhysicalDevice->Destroyed!");
			m_Initialized = false;
		}
	}

	VkPhysicalDevice VulkanPhysicalDevice::GetInternalPhysicalDevice() const
	{
		return m_PhysicalDevice;
	}

	const char* VulkanPhysicalDevice::GetDeviceName() const
	{
		return m_Properties.deviceName;
	}

	const VulkanPhysicalDeviceQueueFamilyIndices& VulkanPhysicalDevice::GetQueueFamilyIndices() const
	{
		return m_QueueFamilyIndices;
	}

	const std::vector<VkDeviceQueueCreateInfo>& VulkanPhysicalDevice::GetRequestedQueueCreateInfos() const
	{
		return m_RequestedQueueCreateInfos;
	}

	const VkPhysicalDeviceFeatures& VulkanPhysicalDevice::GetRequestedFeatures() const
	{
		return m_RequestedFeatures;
	}

	bool VulkanPhysicalDevice::IsExtensionSupported(const std::string& extension) const
	{
		return m_SupportedExtensions.find(extension) != m_SupportedExtensions.end();
	}

	bool VulkanPhysicalDevice::SelectDevice()
	{
		uint32_t gpuDeviceCount;
		VkResult res = vkEnumeratePhysicalDevices(m_Context->GetInstance(), &gpuDeviceCount, nullptr);
		if (res != VK_SUCCESS)
		{
			ATRX_LOG_ERROR("ATRXVulkanPhysicalDevice->Error vkEnumeratePhysicalDevices!");
			return false;
		}
		if (!gpuDeviceCount)
		{
			ATRX_LOG_ERROR("ATRXVulkanPhysicalDevice->Error No GPU Device Supports Vulkan!");
			return false;
		}

		std::vector<VkPhysicalDevice> devices(gpuDeviceCount);
		vkEnumeratePhysicalDevices(m_Context->GetInstance(), &gpuDeviceCount, devices.data());
		for (const auto& device : devices)
		{
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(device, &properties);
			VkPhysicalDeviceMemoryProperties memoryProperties;
			vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);
			VkPhysicalDeviceFeatures features;
			vkGetPhysicalDeviceFeatures(device, &features);

			VulkanPhysicalDeviceRequirements requirements;
			requirements.Discrete = true;
			requirements.Graphics = true;
			requirements.Compute = false;
			requirements.Transfer = true;
			requirements.SamplerAnisotropy = true;
			requirements.ExtensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

			if (CheckDeviceRequirements(requirements, device, properties, memoryProperties, features))
			{
				m_PhysicalDevice = device;
				m_Properties = properties;
				m_MemoryProperties = memoryProperties;
				m_Features = features;
				ATRX_LOG_INFO("ATRXVulkanPhysicalDevice->Selecting Supported GPU({})!", properties.deviceName);
				PrintDeviceProperties(device, properties, memoryProperties, features);
				return true;
			}
		}

		return false;
	}

	bool VulkanPhysicalDevice::CheckDeviceRequirements(const VulkanPhysicalDeviceRequirements& requirements, VkPhysicalDevice device, const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceMemoryProperties& memoryProperties, const VkPhysicalDeviceFeatures& features)
	{
		ATRX_LOG_INFO("ATRXVulkanPhysicalDevice->Checking GPU({}) For: Discrete({}), Graphics Queue({}), Compute Queue({}), Transfer Queue({}), SamplerAnisotropy({})!", properties.deviceName, requirements.Discrete, requirements.Graphics, requirements.Compute, requirements.Transfer, requirements.SamplerAnisotropy);

		if (requirements.Discrete && properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			return false;

		// Check Queues
		VulkanPhysicalDeviceQueueFamilyIndices queueFamilyIndices;
		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyProperties.data());

		size_t idx = 0;
		uint32_t min_transfer_score = 255;
		for (const auto& queue : queueFamilyProperties)
		{
			uint32_t transfer_score = 0;

			if (queue.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				queueFamilyIndices.Graphics = idx;
				transfer_score++;
			}

			if (queue.queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				queueFamilyIndices.Compute = idx;
				transfer_score++;
			}

			if (queue.queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				if (transfer_score <= min_transfer_score)
				{
					min_transfer_score = transfer_score;
					queueFamilyIndices.Transfer = idx;
				}
			}

			idx++;
		}

		if ((!requirements.Graphics || (requirements.Graphics && queueFamilyIndices.Graphics != -1)) &&
			(!requirements.Compute || (requirements.Compute && queueFamilyIndices.Compute != -1)) &&
			(!requirements.Transfer || (requirements.Transfer && queueFamilyIndices.Transfer != -1)))
		{
			ATRX_LOG_DEBUG("ATRXVulkanPhysicalDevice->GPU({}) Queue Details: Discrete({}), Graphics Queue({}), Compute Queue({}), Transfer Queue({})!", properties.deviceName, requirements.Compute, queueFamilyIndices.Graphics, queueFamilyIndices.Compute, queueFamilyIndices.Transfer);
		
			// Check Extensions
			uint32_t extensionCount = 0;
			VkResult res = vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
			if (res != VK_SUCCESS)
			{
				ATRX_LOG_ERROR("ATRXVulkanPhysicalDevice->Error vkEnumerateDeviceExtensionProperties For GPU({}): ({})!", properties.deviceName, (int)res);
				return false;
			}

			if (extensionCount)
			{
				std::vector<VkExtensionProperties> extensions(extensionCount);
				res = vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensions.data());
				if (res != VK_SUCCESS)
				{
					ATRX_LOG_ERROR("ATRXVulkanPhysicalDevice->Error vkEnumerateDeviceExtensionProperties For GPU({})!: ({})!", properties.deviceName, (int)res);
					return false;
				}

				for (const auto& ext : extensions)
					m_SupportedExtensions.emplace(ext.extensionName);

				for (size_t i = 0; i < requirements.ExtensionNames.size(); i++)
				{
					bool flag = false;
					for (size_t j = 0; j < extensions.size(); j++)
					{
						if (!std::strcmp(requirements.ExtensionNames[i], extensions[j].extensionName))
						{
							flag = true;
							ATRX_LOG_INFO("ATRXVulkanPhysicalDevice->GPU({}) Found Vulkan Extension({})!", properties.deviceName, requirements.ExtensionNames[i]);
							break;
						}
					}

					if (!flag)
					{
						ATRX_LOG_ERROR("ATRXVulkanPhysicalDevice->GPU({}) Could not Find Vulkan Extension({})!", properties.deviceName, requirements.ExtensionNames[i]);
						return false;
					}
				}
			}

			// Others
			if (requirements.SamplerAnisotropy)
			{
				if (!features.samplerAnisotropy)
				{
					ATRX_LOG_ERROR("ATRXVulkanPhysicalDevice->GPU({}) Doesn't Support SamplerAnisotropy!", properties.deviceName);
					return false;
				}
				else
				{
					ATRX_LOG_INFO("ATRXVulkanPhysicalDevice->GPU({}) Supports SamplerAnisotropy!", properties.deviceName);
				}
			}

			m_QueueFamilyIndices = queueFamilyIndices;
			return true;
		}

		ATRX_LOG_ERROR("ATRXVulkanPhysicalDevice->GPU({}) Did not Meet Requirements!", properties.deviceName);
		return false;
	}

	void VulkanPhysicalDevice::PrintDeviceProperties(VkPhysicalDevice device, const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceMemoryProperties& memoryProperties, const VkPhysicalDeviceFeatures& features)
	{
		std::string deviceType;
		switch (properties.deviceType)
		{
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			deviceType = "DISCRETE GPU";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			deviceType = "INTEGRATED GPU";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
			deviceType = "VIRTUAL GPU";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_CPU:
			deviceType = "CPU";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_OTHER:
			deviceType = "UNKNOWN";
			break;
		default:
			deviceType = "UNKNOWN";
		}

		ATRX_LOG_INFO("------------------------------GPU({})------------------------------", properties.deviceName);
		ATRX_LOG_INFO("Type({})", deviceType);
		ATRX_LOG_INFO("GPU Driver Version({}.{}.{})", VK_VERSION_MAJOR(properties.driverVersion), VK_VERSION_MINOR(properties.driverVersion), VK_VERSION_PATCH(properties.driverVersion));
		ATRX_LOG_INFO("Vulkan API Version({}.{}.{})", VK_VERSION_MAJOR(properties.apiVersion), VK_VERSION_MINOR(properties.apiVersion), VK_VERSION_PATCH(properties.apiVersion));
	
		for (size_t i = 0; i < memoryProperties.memoryHeapCount; i++)
		{
			size_t memSize = (((float)memoryProperties.memoryHeaps[i].size) / 1024.0f / 1024.0f / 1024.0f);
			if (memoryProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
				ATRX_LOG_INFO("Local GPU Memory({} GB)", memSize);
			else
				ATRX_LOG_INFO("Shared System Memory({} GB)", memSize);
		}
		ATRX_LOG_INFO("------------------------------END({})------------------------------", properties.deviceName);
	}

	void VulkanPhysicalDevice::RequestDeviceQueues(int32_t queueTypes)
	{
		static const float queuePriority = 1.0f;

		if (queueTypes & VK_QUEUE_GRAPHICS_BIT)
		{
			VkDeviceQueueCreateInfo queueInfo{};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = m_QueueFamilyIndices.Graphics;
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &queuePriority;
			m_RequestedQueueCreateInfos.push_back(queueInfo);
		}

		if (queueTypes & VK_QUEUE_COMPUTE_BIT)
		{
			if (m_QueueFamilyIndices.Compute != m_QueueFamilyIndices.Graphics)
			{
				VkDeviceQueueCreateInfo queueInfo{};
				queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueInfo.queueFamilyIndex = m_QueueFamilyIndices.Compute;
				queueInfo.queueCount = 1;
				queueInfo.pQueuePriorities = &queuePriority;
				m_RequestedQueueCreateInfos.push_back(queueInfo);
			}
		}

		if (queueTypes & VK_QUEUE_TRANSFER_BIT)
		{
			if (m_QueueFamilyIndices.Transfer != m_QueueFamilyIndices.Graphics)
			{
				VkDeviceQueueCreateInfo queueInfo{};
				queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueInfo.queueFamilyIndex = m_QueueFamilyIndices.Transfer;
				queueInfo.queueCount = 1;
				queueInfo.pQueuePriorities = &queuePriority;
				m_RequestedQueueCreateInfos.push_back(queueInfo);
			}
		}
	}

	void VulkanPhysicalDevice::RequestDeviceFeatures()
	{
		m_RequestedFeatures.samplerAnisotropy = VK_TRUE;
	}
}
