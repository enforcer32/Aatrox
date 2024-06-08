#pragma once

#include "ATRXEngine/Core/DeltaTime.h"
#include "ATRXEngine/Renderer/API/RendererContext.h"
#include "ATRXEngine/Renderer/API/RendererSurface.h"
#include "ATRXEngine/Renderer/API/RendererDevice.h"

namespace ATRX
{
	struct RenderPacket
	{
		DeltaTime DeltaTime;
	};

	class Renderer
	{
	public:
		bool OnInit(RendererBackendAPI api);
		void OnDestroy();

		bool DrawFrame(RenderPacket packet);
		void SetTargetSurface(const std::shared_ptr<RendererSurface> surface);

		RendererBackendAPI GetBackendAPI() const;
		std::shared_ptr<RendererContext> GetContext() const;
		std::shared_ptr<RendererSurface> GetSurface() const;

	private:
		bool BeginFrame(DeltaTime dt);
		bool EndFrame();

	private:
		bool m_Initialized = false;
		RendererBackendAPI m_BackendAPI;
		std::shared_ptr<RendererContext> m_Context;
		std::shared_ptr<RendererSurface> m_Surface;
		std::shared_ptr<RendererDevice> m_Device;
	};
}
