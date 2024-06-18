#pragma once

#include "ATRXEngine/Renderer/API/RendererSurface.h"
#include "ATRXEngine/Platform/RHI/Vulkan/VulkanContext.h"

namespace ATRX
{
	class VulkanSurface : public RendererSurface
	{
	public:
#ifdef ATRX_PLATFORM_WINDOWS
		virtual bool OnInit(const std::shared_ptr<RendererContext>& context, HINSTANCE hInstance, HWND hwnd) override;
#else
		virtual bool OnInit() override;
#endif // ATRX_PLATFORM_WINDOWS

		virtual void OnDestroy() override;

		VkSurfaceKHR GetInternalSurface() const;
		void SetInternalSurface(VkSurfaceKHR surface);

	private:
		bool m_Initialized = false;
		VkSurfaceKHR m_Surface;
		std::shared_ptr<VulkanContext> m_Context;
	};
}
