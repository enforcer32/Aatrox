#include "ATRXEngine/Renderer/Renderer.h"

namespace ATRX
{
	bool Renderer::OnInit()
	{
		m_RendererBackend = RendererBackend::CreateInstance();
		if (!m_RendererBackend->OnInit())
			return false;
		return m_Initialized = true;
	}

	void Renderer::OnDestroy()
	{
		if(m_Initialized)
		{
			m_RendererBackend->OnDestroy();
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
