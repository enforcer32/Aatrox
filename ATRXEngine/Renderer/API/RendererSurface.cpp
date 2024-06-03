#include "ATRXEngine/Renderer/API/RendererSurface.h"
#include "ATRXEngine/Platform/RHI/Vulkan/VulkanSurface.h"

namespace ATRX
{
	std::shared_ptr<RendererSurface> RendererSurface::CreateInstance(RendererBackendAPI api)
	{
		switch (api)
		{
		case ATRX::RendererBackendAPI::None:
			break;
		case ATRX::RendererBackendAPI::Vulkan:
			return std::make_shared<VulkanSurface>();
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
