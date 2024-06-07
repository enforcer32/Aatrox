#include "ATRXEngine/Platform/RHI/Vulkan/VulkanSurface.h"
#include "ATRXEngine/Core/Logger.h"

#ifdef ATRX_PLATFORM_WINDOWS
#include <vulkan/vulkan_win32.h>
#endif // ATRX_PLATFORM_WINDOWS

namespace ATRX
{
#ifdef ATRX_PLATFORM_WINDOWS
	bool VulkanSurface::OnInit(const std::shared_ptr<RendererContext>& context, HINSTANCE hInstance, HWND hwnd)
	{
		ATRX_LOG_INFO("ATRXVulkanSurface->Initializing Windows Vulkan Surface...");
		m_Context = std::dynamic_pointer_cast<VulkanContext>(context);

		VkWin32SurfaceCreateInfoKHR createInfo = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
		createInfo.hinstance = hInstance;
		createInfo.hwnd = hwnd;

		VkResult res = vkCreateWin32SurfaceKHR(m_Context->GetInstance(), &createInfo, m_Context->GetAllocator(), &m_Surface);
		if (res != VK_SUCCESS)
		{
			ATRX_LOG_ERROR("ATRXVulkanSurface->Error vkCreateWin32SurfaceKHR: ({})!", (int)res);
			return false;
		}

		ATRX_LOG_INFO("ATRXVulkanSurface->Initialized Windows Vulkan Surface!");
		return m_Initialized = true;
	}
#else
	bool VulkanSurface::OnInit()
	{
		ATRX_LOG_INFO("ATRXVulkanSurface->Initializing...");
		ATRX_LOG_INFO("ATRXVulkanSurface->Initialized!");
		return m_Initialized = true;;
	}
#endif // ATRX_PLATFORM_WINDOWS

	void VulkanSurface::OnDestroy()
	{
		if (m_Initialized)
		{
			ATRX_LOG_INFO("ATRXVulkanSurface->Destroying...");
			vkDestroySurfaceKHR(m_Context->GetInstance(), m_Surface, m_Context->GetAllocator());
			m_Context = nullptr;
			ATRX_LOG_INFO("ATRXVulkanSurface->Destroyed!");
			m_Initialized = false;
		}
	}

	VkSurfaceKHR VulkanSurface::GetSurface() const
	{
		return m_Surface;
	}

	void VulkanSurface::SetSurface(VkSurfaceKHR surface)
	{
		m_Surface = surface;
	}
}
