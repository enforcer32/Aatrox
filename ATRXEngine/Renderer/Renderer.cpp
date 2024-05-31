#include "ATRXEngine/Renderer/Renderer.h"
#include "ATRXEngine/Core/Logger.h"

namespace ATRX
{
	bool Renderer::OnInit()
	{
		ATRX_LOG_INFO("ATRXRenderer->Initializing...");
		m_RendererBackend = RendererBackend::CreateInstance();
		if (!m_RendererBackend->OnInit())
		{
			ATRX_LOG_ERROR("ATRXRenderer->Error Initializing RendererBackend!");
			return false;
		}

		ATRX_LOG_INFO("ATRXRenderer->Initialized!");
		return m_Initialized = true;
	}

	void Renderer::OnDestroy()
	{
		if(m_Initialized)
		{
			ATRX_LOG_INFO("ATRXRenderer->Destroying...");
			m_RendererBackend->OnDestroy();
			ATRX_LOG_INFO("ATRXRenderer->Destroyed!");
			m_Initialized = false;
		}
	}

	bool Renderer::DrawFrame(RenderPacket packet)
	{
		if (m_RendererBackend->BeginFrame(packet.DeltaTime)) {
			if (!m_RendererBackend->EndFrame())
				return false;
		}
		return true;
	}
}
