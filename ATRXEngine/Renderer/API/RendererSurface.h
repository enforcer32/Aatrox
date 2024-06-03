#pragma once

#include "ATRXEngine/Renderer/API/RendererBackend.h"
#include "ATRXEngine/Renderer/API/RendererContext.h"
#include "ATRXEngine/Core/Platform.h"

#include <memory>

#ifdef ATRX_PLATFORM_WINDOWS
#include <Windows.h>
#endif // ATRX_PLATFORM_WINDOWS

namespace ATRX
{
	class RendererSurface
	{
	public:
		static std::shared_ptr<RendererSurface> CreateInstance(RendererBackendAPI api);

		virtual ~RendererSurface() = default;

#ifdef ATRX_PLATFORM_WINDOWS
		virtual bool OnInit(const std::shared_ptr<RendererContext>& context, HINSTANCE hInstance, HWND hwnd) = 0;
#else
		virtual bool OnInit() = 0;
#endif // ATRX_PLATFORM_WINDOWS
		virtual void OnDestroy() = 0;
	};
}
