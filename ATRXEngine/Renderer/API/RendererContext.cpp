#include "ATRXEngine/Renderer/API/RendererContext.h"
#include "ATRXEngine/Platform/RHI/Vulkan/VulkanContext.h"

namespace ATRX
{
	std::shared_ptr<RendererContext> RendererContext::CreateInstance(RendererBackendAPI api)
	{
		switch (api)
		{
		case ATRX::RendererBackendAPI::None:
			break;
		case ATRX::RendererBackendAPI::Vulkan:
			return std::make_shared<VulkanContext>();
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
