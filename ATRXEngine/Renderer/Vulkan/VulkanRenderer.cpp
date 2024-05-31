#include "ATRXEngine/Renderer/Vulkan/VulkanRenderer.h"

namespace ATRX
{
	RendererBackendAPI RendererBackend::s_BackendAPI = RendererBackendAPI::Vulkan;

	std::unique_ptr<RendererBackend> RendererBackend::CreateInstance()
	{
		return std::make_unique<VulkanRenderer>();
	}

	bool VulkanRenderer::OnInit()
	{
		m_Context = std::make_shared<VulkanContext>();
		if (!m_Context->OnInit())
			return false;

		return m_Initialized = true;
	}

	void VulkanRenderer::OnDestroy()
	{
		if (m_Initialized)
		{
			m_Context->OnDestroy();
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
