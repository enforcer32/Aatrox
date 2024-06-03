#pragma once

#include "ATRXEngine/Renderer/API/RendererBackend.h"

#include <memory>

namespace ATRX
{
	class RendererContext
	{
	public:
		static std::shared_ptr<RendererContext> CreateInstance(RendererBackendAPI api);

		virtual ~RendererContext() = default;

		virtual bool OnInit() = 0;
		virtual void OnDestroy() = 0;
	};
}
