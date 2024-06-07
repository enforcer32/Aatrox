#pragma once

#include "ATRXEngine/Renderer/API/RendererDevice.h"
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

	class VulkanPhysicalDevice : public RendererDevice
	{
	public:
		virtual bool OnInit(const std::shared_ptr<RendererContext>& context) override;
		virtual void OnDestroy() override;

	private:
		bool SelectDevice();
		bool CheckDeviceRequirements(const VulkanPhysicalDeviceRequirements& requirements, VkPhysicalDevice device, const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceMemoryProperties& memoryProperties, const VkPhysicalDeviceFeatures& features);
		void PrintDeviceProperties(VkPhysicalDevice device, const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceMemoryProperties& memoryProperties, const VkPhysicalDeviceFeatures& features);

	private:
		bool m_Initialized = false;
		std::shared_ptr<VulkanContext> m_Context;
		VkPhysicalDevice m_PhysicalDevice;
		VkPhysicalDeviceProperties m_Properties;
		VkPhysicalDeviceMemoryProperties m_MemoryProperties;
		VkPhysicalDeviceFeatures m_Features;
		VulkanPhysicalDeviceQueueFamilyIndices m_QueueFamilyIndices;
	};
}
