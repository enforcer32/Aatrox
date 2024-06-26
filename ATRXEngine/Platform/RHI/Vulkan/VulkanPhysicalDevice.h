#pragma once

#include "ATRXEngine/Platform/RHI/Vulkan/VulkanContext.h"

#include <memory>
#include <unordered_set>
#include <string>
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
		const char* GetDeviceName() const;
		const VulkanPhysicalDeviceQueueFamilyIndices& GetQueueFamilyIndices() const;
		const std::vector<VkDeviceQueueCreateInfo>& GetRequestedQueueCreateInfos() const;
		const VkPhysicalDeviceFeatures& GetRequestedFeatures() const;
		VkFormat GetDepthFormat() const;
		int32_t GetMemoryTypeIndex(uint32_t memoryTypeBits, VkMemoryPropertyFlags properties);
		bool IsExtensionSupported(const std::string& extension) const;

	private:
		bool SelectDevice();
		bool CheckDeviceRequirements(const VulkanPhysicalDeviceRequirements& requirements, VkPhysicalDevice device, const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceMemoryProperties& memoryProperties, const VkPhysicalDeviceFeatures& features);
		void PrintDeviceProperties(VkPhysicalDevice device, const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceMemoryProperties& memoryProperties, const VkPhysicalDeviceFeatures& features);
		void RequestDeviceQueues(int32_t queueTypes);
		void RequestDeviceFeatures();
		VkFormat DetectDepthFormat() const;

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
		std::unordered_set<std::string> m_SupportedExtensions;
		VkFormat m_DepthFormat = VK_FORMAT_UNDEFINED;
	};
}
