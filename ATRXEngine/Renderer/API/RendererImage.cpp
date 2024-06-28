#include "ATRXEngine/Renderer/API/RendererImage.h"
#include "ATRXEngine/Platform/RHI/Vulkan/VulkanImage.h"

namespace ATRX
{
	std::shared_ptr<RendererImage> RendererImage::CreateInstance(RendererBackendAPI api)
	{
		switch (api)
		{
		case ATRX::RendererBackendAPI::None:
			break;
		case ATRX::RendererBackendAPI::Vulkan:
			return std::make_shared<VulkanImage>();
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
