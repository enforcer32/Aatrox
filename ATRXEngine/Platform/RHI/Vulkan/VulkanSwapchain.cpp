#include "ATRXEngine/Platform/RHI/Vulkan/VulkanSwapchain.h"
#include "ATRXEngine/Core/Logger.h"

namespace ATRX
{
	bool VulkanSwapchain::OnInit(const std::shared_ptr<RendererContext>& context, const std::shared_ptr<RendererDevice>& device, const std::shared_ptr<RendererSurface>& surface)
	{
		m_Context = std::dynamic_pointer_cast<VulkanContext>(context);
		m_Device = std::dynamic_pointer_cast<VulkanDevice>(device);
		m_Surface = std::dynamic_pointer_cast<VulkanSurface>(surface);

		if (!InitSurfaceAndPresent())
		{
			ATRX_LOG_ERROR("ATRXVulkanSwapchain->Error InitSurfaceAndPresent!");
			return false;
		}

		return m_Initialized = true;
	}

	void VulkanSwapchain::OnDestroy()
	{
	}

	void VulkanSwapchain::Create(uint32_t width, uint32_t height, bool vsync)
	{

	}

	void VulkanSwapchain::Present()
	{
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
	}

	bool VulkanSwapchain::InitSurfaceAndPresent()
	{
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
