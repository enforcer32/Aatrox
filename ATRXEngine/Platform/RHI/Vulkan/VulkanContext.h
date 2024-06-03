#pragma once

#include "ATRXEngine/Renderer/API/RendererContext.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace ATRX
{
	class VulkanContext : public RendererContext
	{
	public:
		virtual bool OnInit() override;
		virtual void OnDestroy() override;

		VkInstance GetInstance() const;
		VkAllocationCallbacks* GetAllocator() const;

	private:
		bool InitVulkanExtensions();
		bool InitVulkanValidationLayers();
		bool InitVulkanDebugger();
		void EnableVulkanExtension(const char* extension);
		void EnableVulkanValidationLayer(const char* validationLayer);

	private:
		bool m_Initialized = false;
		bool m_ValidationLayerEnabled = false;
		VkInstance m_Instance;
		VkAllocationCallbacks* m_Allocator = nullptr;
		VkDebugUtilsMessengerEXT m_DebugMessenger = nullptr;
		std::vector<const char*> m_VulkanExtensions;
		std::vector<const char*> m_VulkanValidationLayers;
	};
}
