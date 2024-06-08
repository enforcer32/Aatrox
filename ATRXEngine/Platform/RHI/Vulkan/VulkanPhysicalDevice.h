#pragma once

#include "ATRXEngine/Platform/RHI/Vulkan/VulkanContext.h"

#include <memory>
#include <vulkan/vulkan.h>

namespace ATRX
{
	struct VulkanPhysicalDeviceRequirements
	{
		bool Discrete;
		bool Graphics;
		bool Compute;
		bool Transfer;
		bool SamplerAnisotropy;
		std::vector<const char*> ExtensionNames;
	};

	struct VulkanPhysicalDeviceQueueFamilyIndices
	{
		int32_t Graphics = -1;
		int32_t Compute = -1;
		int32_t Transfer = -1;
	};

	class VulkanPhysicalDevice
	{
	public:
		bool OnInit(const std::shared_ptr<VulkanContext>& context);
		void OnDestroy();

		VkPhysicalDevice GetInternalPhysicalDevice() const;
		const VulkanPhysicalDeviceQueueFamilyIndices& GetQueueFamilyIndices() const;
		const std::vector<VkDeviceQueueCreateInfo>& GetRequestedQueueCreateInfos() const;
		const VkPhysicalDeviceFeatures& GetRequestedFeatures() const;

	private:
		bool SelectDevice();
		bool CheckDeviceRequirements(const VulkanPhysicalDeviceRequirements& requirements, VkPhysicalDevice device, const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceMemoryProperties& memoryProperties, const VkPhysicalDeviceFeatures& features);
		void PrintDeviceProperties(VkPhysicalDevice device, const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceMemoryProperties& memoryProperties, const VkPhysicalDeviceFeatures& features);
		void RequestDeviceQueues(int32_t queueTypes);
		void RequestDeviceFeatures();

	private:
		bool m_Initialized = false;
		std::shared_ptr<VulkanContext> m_Context;
		VkPhysicalDevice m_PhysicalDevice;
		VkPhysicalDeviceProperties m_Properties;
		VkPhysicalDeviceMemoryProperties m_MemoryProperties;
		VkPhysicalDeviceFeatures m_Features;
		VulkanPhysicalDeviceQueueFamilyIndices m_QueueFamilyIndices;
		std::vector<VkDeviceQueueCreateInfo> m_RequestedQueueCreateInfos;
		VkPhysicalDeviceFeatures m_RequestedFeatures;
	};
}
