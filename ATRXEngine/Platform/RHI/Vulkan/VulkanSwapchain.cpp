#include "ATRXEngine/Platform/RHI/Vulkan/VulkanSwapchain.h"
#include "ATRXEngine/Platform/RHI/Vulkan/VulkanImage.h"
#include "ATRXEngine/Core/Logger.h"

#include <algorithm>

namespace ATRX
{
	bool VulkanSwapchain::OnInit(const std::shared_ptr<RendererContext>& context, const std::shared_ptr<RendererDevice>& device, const std::shared_ptr<RendererSurface>& surface)
	{
		ATRX_LOG_INFO("ATRXVulkanSwapchain->Initializing...");
		m_Context = std::dynamic_pointer_cast<VulkanContext>(context);
		m_Device = std::dynamic_pointer_cast<VulkanDevice>(device);
		m_Surface = std::dynamic_pointer_cast<VulkanSurface>(surface);

		if (!InitSurfaceAndPresent())
		{
			ATRX_LOG_ERROR("ATRXVulkanSwapchain->Error InitSurfaceAndPresent!");
			return false;
		}

		// Temporary Values: CONFIG
		uint32_t width = 800, height = 600;
		Create(width, height, false);

		ATRX_LOG_INFO("ATRXVulkanSwapchain->Initialized!");
		return m_Initialized = true;
	}

	void VulkanSwapchain::OnDestroy()
	{
		ATRX_LOG_INFO("ATRXVulkanSwapchain->Destroying...");
		m_DepthAttachment->OnDestroy();
		for (const auto& image : m_Images)
			vkDestroyImageView(m_Device->GetInternalDevice(), image.ImageView, m_Context->GetAllocator());
		vkDestroySwapchainKHR(m_Device->GetInternalDevice(), m_Swapchain, m_Context->GetAllocator());
		ATRX_LOG_INFO("ATRXVulkanSwapchain->Destroyed!");
	}

	void VulkanSwapchain::Create(uint32_t width, uint32_t height, bool vsync)
	{
		ATRX_LOG_INFO("ATRXVulkanSwapchain->Creating...");
		VkExtent2D swapchainExtent = { width, height };

		// Surface Format
		m_ImageFormat = m_SwapchainCapabilities.SurfaceFormats[0];
		for (const auto& format : m_SwapchainCapabilities.SurfaceFormats)
		{
			if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				m_ImageFormat = format;
				break;
			}
		}

		// Present Mode
		VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
		for (const auto& mode : m_SwapchainCapabilities.PresentModes)
		{
			if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				presentMode = VK_PRESENT_MODE_FIFO_KHR;
				break;
			}
		}

		// Update Swapchain
		FetchSwapchainCapabilities();

		if (m_SwapchainCapabilities.SurfaceCapabilities.currentExtent.width != UINT32_MAX)
			swapchainExtent = m_SwapchainCapabilities.SurfaceCapabilities.currentExtent;
		
		VkExtent2D min = m_SwapchainCapabilities.SurfaceCapabilities.minImageExtent;
		VkExtent2D max = m_SwapchainCapabilities.SurfaceCapabilities.maxImageExtent;
		swapchainExtent.width = std::clamp(swapchainExtent.width, min.width, max.width);
		swapchainExtent.height = std::clamp(swapchainExtent.height, min.height, max.height);
		
		uint32_t imageCount = m_SwapchainCapabilities.SurfaceCapabilities.minImageCount + 1;
		if (m_SwapchainCapabilities.SurfaceCapabilities.maxImageCount > 0 && imageCount > m_SwapchainCapabilities.SurfaceCapabilities.maxImageCount)
			imageCount = m_SwapchainCapabilities.SurfaceCapabilities.maxImageCount;
	
		VkSwapchainCreateInfoKHR swapchainCreateInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
		swapchainCreateInfo.surface = m_Surface->GetInternalSurface();
		swapchainCreateInfo.imageExtent = swapchainExtent;
		swapchainCreateInfo.minImageCount = imageCount;
		swapchainCreateInfo.imageFormat = m_ImageFormat.format;
		swapchainCreateInfo.imageColorSpace = m_ImageFormat.colorSpace;
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		// Setup Queues
		if (m_Device->GetPhysicalDevice()->GetQueueFamilyIndices().Graphics != m_PresentQueueIndex)
		{
			uint32_t queueFamilyIndices[] = { m_Device->GetPhysicalDevice()->GetQueueFamilyIndices().Graphics, m_PresentQueueIndex };
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapchainCreateInfo.queueFamilyIndexCount = 2;
			swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swapchainCreateInfo.queueFamilyIndexCount = 0;
			swapchainCreateInfo.pQueueFamilyIndices = 0;
		}

		swapchainCreateInfo.preTransform = m_SwapchainCapabilities.SurfaceCapabilities.currentTransform;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.presentMode = presentMode;
		swapchainCreateInfo.clipped = VK_TRUE;
		swapchainCreateInfo.oldSwapchain = 0;

		VkResult res = vkCreateSwapchainKHR(m_Device->GetInternalDevice(), &swapchainCreateInfo, m_Context->GetAllocator(), &m_Swapchain);
		if (res != VK_SUCCESS)
		{
			ATRX_LOG_ERROR("ATRXVulkanSwapchain->Error vkCreateSwapchainKHR: ({})!", (int)res);
			return;
		}

		// Setup Images
		imageCount = 0;
		res = vkGetSwapchainImagesKHR(m_Device->GetInternalDevice(), m_Swapchain, &imageCount, nullptr);
		if (res != VK_SUCCESS)
		{
			ATRX_LOG_ERROR("ATRXVulkanSwapchain->Error vkGetSwapchainImagesKHR: ({})!", (int)res);
			return;
		}

		m_VulkanImages.resize(imageCount);
		res = vkGetSwapchainImagesKHR(m_Device->GetInternalDevice(), m_Swapchain, &imageCount, m_VulkanImages.data());
		if (res != VK_SUCCESS)
		{
			ATRX_LOG_ERROR("ATRXVulkanSwapchain->Error vkGetSwapchainImagesKHR: ({})!", (int)res);
			return;
		}

		m_Images.resize(imageCount);
		for (size_t i = 0; i < m_VulkanImages.size(); i++)
		{
			VkImageViewCreateInfo imageViewCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
			imageViewCreateInfo.image = m_VulkanImages[i];
			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewCreateInfo.format = m_ImageFormat.format;
			imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;

			m_Images[i].Image = m_VulkanImages[i];

			res = vkCreateImageView(m_Device->GetInternalDevice(), &imageViewCreateInfo, m_Context->GetAllocator(), &m_Images[i].ImageView);
			if (res != VK_SUCCESS)
			{
				ATRX_LOG_ERROR("ATRXVulkanSwapchain->Error vkCreateImageView: ({})!", (int)res);
				return;
			}
		}

		// Depth Format
		VkFormat depthFormat = m_Device->GetPhysicalDevice()->GetDepthFormat();
		if (depthFormat == VK_FORMAT_UNDEFINED)
		{
			ATRX_LOG_ERROR("ATRXVulkanSwapchain->Error depthFormat VK_FORMAT_UNDEFINED!");
			return;
		}

		RendererImageProperties depthImageProperties;
		depthImageProperties.Format = depthFormat;
		depthImageProperties.Usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		depthImageProperties.Width = swapchainExtent.width;
		depthImageProperties.Height = swapchainExtent.height;
		depthImageProperties.Depth = 1;
		depthImageProperties.Mips = 4;
		depthImageProperties.Layers = 1;
		depthImageProperties.AspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
		depthImageProperties.MemoryType = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		m_DepthAttachment = std::make_shared<VulkanImage>();
		if (!m_DepthAttachment->OnInit(m_Context, m_Device, depthImageProperties))
		{
			ATRX_LOG_ERROR("ATRXVulkanSwapchain->Error m_DepthAttachment OnInit!");
			return;
		}

		ATRX_LOG_INFO("ATRXVulkanSwapchain->Created!");
	}

	void VulkanSwapchain::Present()
	{
		ATRX_LOG_INFO("ATRXVulkanSwapchain->Presenting...");
		VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &m_Semaphores.Render;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_Swapchain;
		presentInfo.pImageIndices = &m_CurrentImageIndex;

		VkResult res = vkQueuePresentKHR(m_Device->GetGraphicsQueue(), &presentInfo); // Present Queue Instead?
		if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR)
		{
			ATRX_LOG_ERROR("ATRXVulkanSwapchain->Error vkQueuePresentKHR({}) VK_ERROR_OUT_OF_DATE_KHR OR VK_SUBOPTIMAL_KHR ReCreating Swapchain!", (int)res);
			Create(1, 1, false); // TEMPORARY
			return;
		}
		else if (res != VK_SUCCESS)
		{
			ATRX_LOG_ERROR("ATRXVulkanSwapchain->Error vkQueuePresentKHR: ({})!", (int)res);
			return;
		}

		ATRX_LOG_INFO("ATRXVulkanSwapchain->Presented!");
	}

	bool VulkanSwapchain::InitSurfaceAndPresent()
	{
		ATRX_LOG_INFO("ATRXVulkanSwapchain->Initializing SurfaceAndPresent...");
		const VulkanPhysicalDeviceQueueFamilyIndices& queueIndices = m_Device->GetPhysicalDevice()->GetQueueFamilyIndices();

		// Setup Present Queue
		uint32_t favoredPresentQueueIdx = UINT32_MAX;

		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(m_Device->GetPhysicalDevice()->GetInternalPhysicalDevice(), &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_Device->GetPhysicalDevice()->GetInternalPhysicalDevice(), &queueFamilyCount, queueFamilyProperties.data());

		// Queues that Support Present Mode
		std::vector<VkBool32> supportsPresent(queueFamilyCount);
		for (size_t i = 0; i < queueFamilyCount; i++)
		{
			VkResult res = vkGetPhysicalDeviceSurfaceSupportKHR(m_Device->GetPhysicalDevice()->GetInternalPhysicalDevice(), i, m_Surface->GetInternalSurface(), &supportsPresent[i]);
			if (res != VK_SUCCESS)
			{
				ATRX_LOG_ERROR("ATRXVulkanSwapchain->Error vkGetPhysicalDeviceSurfaceSupportKHR For GPU({}): ({})!", m_Device->GetDeviceName(), (int)res);
				return false;
			}

			if (supportsPresent[i])
				favoredPresentQueueIdx = i;
		}

		// If Possible: Queue that Supports Both Graphics & Present
		if(favoredPresentQueueIdx != queueIndices.Graphics)
			for (size_t i = 0; i < queueFamilyCount; i++)
				if (supportsPresent[i] && (i == queueIndices.Graphics))
					favoredPresentQueueIdx = queueIndices.Graphics;

		if (!CheckSwapchainCapabilities())
		{
			ATRX_LOG_ERROR("ATRXVulkanSwapchain->Error CheckSwapchainCapabilities, GPU({}) NOT SUPPORTED!", m_Device->GetDeviceName());
			return false;
		}

		m_PresentQueueIndex = favoredPresentQueueIdx;
		ATRX_LOG_INFO("ATRXVulkanSwapchain->Initialized SurfaceAndPresent!");
		return true;
	}

	void VulkanSwapchain::FetchSwapchainCapabilities()
	{
		// Surface Capabilities
		VkResult res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_Device->GetPhysicalDevice()->GetInternalPhysicalDevice(), m_Surface->GetInternalSurface(), &m_SwapchainCapabilities.SurfaceCapabilities);
		if (res != VK_SUCCESS)
		{
			ATRX_LOG_ERROR("ATRXVulkanSwapchain->Error vkGetPhysicalDeviceSurfaceCapabilitiesKHR: ({})!", (int)res);
			return;
		}

		// Surface Formats
		uint32_t formatsCount = 0;
		res = vkGetPhysicalDeviceSurfaceFormatsKHR(m_Device->GetPhysicalDevice()->GetInternalPhysicalDevice(), m_Surface->GetInternalSurface(), &formatsCount, nullptr);
		if (res != VK_SUCCESS)
		{
			ATRX_LOG_ERROR("ATRXVulkanSwapchain->Error vkGetPhysicalDeviceSurfaceFormatsKHR: ({})!", (int)res);
			return;
		}

		if (formatsCount != 0)
		{
			m_SwapchainCapabilities.SurfaceFormats.resize(formatsCount);
			res = vkGetPhysicalDeviceSurfaceFormatsKHR(m_Device->GetPhysicalDevice()->GetInternalPhysicalDevice(), m_Surface->GetInternalSurface(), &formatsCount, m_SwapchainCapabilities.SurfaceFormats.data());
			if (res != VK_SUCCESS)
			{
				ATRX_LOG_ERROR("ATRXVulkanSwapchain->Error vkGetPhysicalDeviceSurfaceFormatsKHR: ({})!", (int)res);
				return;
			}
		}
		
		// Present Modes
		uint32_t presentModesCount = 0;
		res = vkGetPhysicalDeviceSurfacePresentModesKHR(m_Device->GetPhysicalDevice()->GetInternalPhysicalDevice(), m_Surface->GetInternalSurface(), &presentModesCount, nullptr);
		if (res != VK_SUCCESS)
		{
			ATRX_LOG_ERROR("ATRXVulkanSwapchain->Error vkGetPhysicalDeviceSurfacePresentModesKHR: ({})!", (int)res);
			return;
		}

		if (presentModesCount != 0)
		{
			m_SwapchainCapabilities.PresentModes.resize(presentModesCount);
			res = vkGetPhysicalDeviceSurfacePresentModesKHR(m_Device->GetPhysicalDevice()->GetInternalPhysicalDevice(), m_Surface->GetInternalSurface(), &presentModesCount, m_SwapchainCapabilities.PresentModes.data());
			if (res != VK_SUCCESS)
			{
				ATRX_LOG_ERROR("ATRXVulkanSwapchain->Error vkGetPhysicalDeviceSurfacePresentModesKHR: ({})!", (int)res);
				return;
			}
		}
	}

	bool VulkanSwapchain::CheckSwapchainCapabilities()
	{
		FetchSwapchainCapabilities();

		if (m_SwapchainCapabilities.SurfaceFormats.size() < 1 || m_SwapchainCapabilities.PresentModes.size() < 1)
		{
			ATRX_LOG_ERROR("ATRXVulkanSwapchain->Error CheckSwapchainCapabilities SurfaceFormatCount({}), PresentModesCount({})!", m_SwapchainCapabilities.SurfaceFormats.size(), m_SwapchainCapabilities.PresentModes.size());
			return false;
		}
		
		return true;
	}

	std::optional<uint32_t> VulkanSwapchain::AcquireNextImage()
	{
		uint32_t imageIndex;
		VkResult res = vkAcquireNextImageKHR(m_Device->GetInternalDevice(), m_Swapchain, UINT64_MAX, m_Semaphores.Present, m_Fence, &imageIndex);
		if (res == VK_ERROR_OUT_OF_DATE_KHR)
		{
			ATRX_LOG_ERROR("ATRXVulkanSwapchain->Error vkAcquireNextImageKHR({}) VK_ERROR_OUT_OF_DATE_KHR ReCreating Swapchain!", (int)res);
			Create(1, 1, false); // TEMPORARY
			return std::nullopt;
		}
		else if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR)
		{
			ATRX_LOG_ERROR("ATRXVulkanSwapchain->Error vkAcquireNextImageKHR: ({})!", (int)res);
			return std::nullopt;
		}

		return imageIndex;
	}
}
