#pragma once

#include "ATRXEngine/Renderer/API/RendererSwapchain.h"
#include "ATRXEngine/Platform/RHI/Vulkan/VulkanContext.h"
#include "ATRXEngine/Platform/RHI/Vulkan/VulkanDevice.h"
#include "ATRXEngine/Platform/RHI/Vulkan/VulkanSurface.h"
#include "ATRXEngine/Platform/RHI/Vulkan/VulkanImage.h"

#include <vulkan/vulkan.h>
#include <optional>

namespace ATRX
{
	struct VulkanSwapchainImage
	{
		VkImage Image;
		VkImageView ImageView;
	};

	struct VulkanSwapchainSemaphore
	{
		VkSemaphore Present;
		VkSemaphore Render;
	};

	struct VulkanSwapchainCapabilities
	{
		VkSurfaceCapabilitiesKHR SurfaceCapabilities;
		std::vector<VkSurfaceFormatKHR> SurfaceFormats;
		std::vector<VkPresentModeKHR> PresentModes;
	};

	class VulkanSwapchain : public RendererSwapchain
	{
	public:
		virtual bool OnInit(const std::shared_ptr<RendererContext>& context, const std::shared_ptr<RendererDevice>& device, const std::shared_ptr<RendererSurface>& surface) override;
		virtual void OnDestroy() override;

		void Create(uint32_t width, uint32_t height, bool vsync);
		void Present();

	private:
		bool InitSurfaceAndPresent();
		void FetchSwapchainCapabilities();
		bool CheckSwapchainCapabilities();
		std::optional<uint32_t> AcquireNextImage();

	private:
		bool m_Initialized = false;
		std::shared_ptr<VulkanContext> m_Context;
		std::shared_ptr<VulkanDevice> m_Device;
		std::shared_ptr<VulkanSurface> m_Surface;
		VulkanSwapchainCapabilities m_SwapchainCapabilities;
		VkSwapchainKHR m_Swapchain;
		VkSurfaceFormatKHR m_ImageFormat;
		std::vector<VkImage> m_VulkanImages;
		std::vector<VulkanSwapchainImage> m_Images;
		VulkanSwapchainSemaphore m_Semaphores;
		VkFence m_Fence;
		uint32_t m_CurrentImageIndex;
		uint32_t m_PresentQueueIndex;
		std::shared_ptr<VulkanImage> m_DepthAttachment;
	};
}
