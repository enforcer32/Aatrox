#pragma once

#include "ATRXEngine/Renderer/API/RendererBackend.h"
#include "ATRXEngine/Renderer/API/RendererContext.h"

namespace ATRX
{
	class RendererDevice
	{
	public:
		static std::shared_ptr<RendererDevice> CreateInstance(RendererBackendAPI api);

		virtual ~RendererDevice() = default;

		virtual bool OnInit(const std::shared_ptr<RendererContext>& context) = 0;
		virtual void OnDestroy() = 0;
	};
}
