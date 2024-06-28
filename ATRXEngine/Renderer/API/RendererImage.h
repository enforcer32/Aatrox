#pragma once

#include "ATRXEngine/Renderer/API/RendererBackend.h"
#include "ATRXEngine/Renderer/API/RendererContext.h"
#include "ATRXEngine/Renderer/API/RendererDevice.h"

#include <memory>

namespace ATRX
{
	struct RendererImageProperties
	{
		uint32_t Format;
		uint32_t Usage;
		uint32_t Width;
		uint32_t Height;
		uint32_t Depth;
		uint32_t Mips;
		uint32_t Layers;
		uint32_t AspectFlags;
		uint32_t MemoryType;
	};

	class RendererImage
	{
	public:
		static std::shared_ptr<RendererImage> CreateInstance(RendererBackendAPI api);

		virtual ~RendererImage() = default;

		virtual bool OnInit(const std::shared_ptr<RendererContext>& context, const std::shared_ptr<RendererDevice>& device, const RendererImageProperties& imageProperties) = 0;
		virtual void OnDestroy() = 0;
	};
}
