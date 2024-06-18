#include "ATRXEngine/Renderer/API/RendererSwapchain.h"
#include "ATRXEngine/Platform/RHI/Vulkan/VulkanSwapchain.h"

namespace ATRX
{
	std::shared_ptr<RendererSwapchain> RendererSwapchain::CreateInstance(RendererBackendAPI api)
	{
		switch (api)
		{
		case ATRX::RendererBackendAPI::None:
			break;
		case ATRX::RendererBackendAPI::Vulkan:
			return std::make_shared<VulkanSwapchain>();
			break;
		case ATRX::RendererBackendAPI::DirectX:
			break;
		case ATRX::RendererBackendAPI::OpenGL:
			break;
		default:
			break;
		}
		return nullptr;
	}
}
