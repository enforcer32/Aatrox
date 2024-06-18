#pragma once

#include "ATRXEngine/Renderer/API/RendererBackend.h"
#include "ATRXEngine/Renderer/API/RendererDevice.h"
#include "ATRXEngine/Renderer/API/RendererSurface.h"

namespace ATRX
{
	class RendererSwapchain
	{
	public:
		static std::shared_ptr<RendererSwapchain> CreateInstance(RendererBackendAPI api);

		virtual ~RendererSwapchain() = default;

		virtual bool OnInit(const std::shared_ptr<RendererContext>& context, const std::shared_ptr<RendererDevice>& device, const std::shared_ptr<RendererSurface>& surface) = 0;
		virtual void OnDestroy() = 0;
	};
}
