#include "ATRXEngine/Renderer/API/RendererDevice.h"
#include "ATRXEngine/Platform/RHI/Vulkan/VulkanDevice.h"

namespace ATRX
{
	std::shared_ptr<RendererDevice> RendererDevice::CreateInstance(RendererBackendAPI api)
	{
		switch (api)
		{
		case ATRX::RendererBackendAPI::None:
			break;
		case ATRX::RendererBackendAPI::Vulkan:
			return std::make_shared<VulkanDevice>();
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
