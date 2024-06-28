#pragma once

#include "ATRXEngine/Renderer/API/RendererImage.h"
#include "ATRXEngine/Platform/RHI/Vulkan/VulkanContext.h"
#include "ATRXEngine/Platform/RHI/Vulkan/VulkanDevice.h"

#include <vulkan/vulkan.h>

namespace ATRX
{
	class VulkanImage : public RendererImage
	{
	public:
		virtual bool OnInit(const std::shared_ptr<RendererContext>& context, const std::shared_ptr<RendererDevice>& device, const RendererImageProperties& imageProperties) override;
		virtual void OnDestroy() override;

	private:
		bool CreateImage(bool createImageView);
		bool CreateImageView();

	private:
		bool m_Initialized = false;
		std::shared_ptr<VulkanContext> m_Context;
		std::shared_ptr<VulkanDevice> m_Device;
		RendererImageProperties m_ImageProperties;
		VkImage m_Image;
		VkImageView m_ImageView;
		VkDeviceMemory m_ImageMemory;
	};
}
