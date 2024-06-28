#include "ATRXEngine/Renderer/Renderer.h"
#include "ATRXEngine/Core/Logger.h"

namespace ATRX
{
	bool Renderer::OnInit(RendererBackendAPI api)
	{
		ATRX_LOG_INFO("ATRXRenderer->Initializing...");
		m_BackendAPI = api;

		m_Context = RendererContext::CreateInstance(m_BackendAPI);
		if (!m_Context->OnInit())
		{
			ATRX_LOG_ERROR("ATRXRenderer->Error Initializing RendererContext!");
			return false;
		}

		m_Device = RendererDevice::CreateInstance(api);
		if (!m_Device->OnInit(m_Context))
		{
			ATRX_LOG_ERROR("ATRXRenderer->Error Initializing RendererDevice!");
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
			m_Swapchain->OnDestroy();
			if (m_Surface)
				m_Surface->OnDestroy();
			m_Device->OnDestroy();
			m_Context->OnDestroy();
			ATRX_LOG_INFO("ATRXRenderer->Destroyed!");
			m_Initialized = false;
		}
	}

	bool Renderer::DrawFrame(RenderPacket packet)
	{
		if (BeginFrame(packet.DeltaTime)) {
			if (!EndFrame())
				return false;
		}
		return true;
	}

	void Renderer::SetTargetSurface(const std::shared_ptr<RendererSurface> surface)
	{
		if (surface == nullptr)
			ATRX_LOG_ERROR("ATRXRenderer->SetTargetSurface Invalid Surface!");
		else
			m_Surface = surface;
	}

	bool Renderer::CreateSwapchain()
	{
		m_Swapchain = RendererSwapchain::CreateInstance(m_BackendAPI);
		if (!m_Swapchain->OnInit(m_Context, m_Device, m_Surface))
		{
			ATRX_LOG_ERROR("ATRXRenderer->Error Initializing RendererSwapchain!");
			return false;
		}
		return true;
	}

	RendererBackendAPI Renderer::GetBackendAPI() const
	{
		return m_BackendAPI;
	}

	std::shared_ptr<RendererContext> Renderer::GetContext() const
	{
		return m_Context;
	}

	std::shared_ptr<RendererSurface> Renderer::GetSurface() const
	{
		return m_Surface;
	}

	bool Renderer::BeginFrame(DeltaTime dt)
	{
		return true;
	}

	bool Renderer::EndFrame()
	{
		return true;
	}
}
