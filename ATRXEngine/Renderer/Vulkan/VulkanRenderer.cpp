#include "ATRXEngine/Renderer/Vulkan/VulkanRenderer.h"
#include "ATRXEngine/Core/Logger.h"

namespace ATRX
{
	RendererBackendAPI RendererBackend::s_BackendAPI = RendererBackendAPI::Vulkan;

	std::unique_ptr<RendererBackend> RendererBackend::CreateInstance()
	{
		return std::make_unique<VulkanRenderer>();
	}

	bool VulkanRenderer::OnInit()
	{
		ATRX_LOG_INFO("ATRXVulkanRenderer->Initializing...");
		m_Context = std::make_shared<VulkanContext>();
		if (!m_Context->OnInit())
		{
			ATRX_LOG_ERROR("ATRXVulkanRenderer->Error Initializing VulkanContext");
			return false;
		}

		ATRX_LOG_INFO("ATRXVulkanRenderer->Initialized!");
		return m_Initialized = true;
	}

	void VulkanRenderer::OnDestroy()
	{
		if (m_Initialized)
		{
			ATRX_LOG_INFO("ATRXVulkanRenderer->Destroying...");
			m_Context->OnDestroy();
			ATRX_LOG_INFO("ATRXVulkanRenderer->Destroyed!");
			m_Initialized = false;
		}
	}

	bool VulkanRenderer::BeginFrame(DeltaTime dt)
	{
		return true;
	}

	bool VulkanRenderer::EndFrame()
	{
		return true;
	}
}
