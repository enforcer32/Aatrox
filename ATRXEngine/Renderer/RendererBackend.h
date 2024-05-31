#pragma once

#include "ATRXEngine/Core/DeltaTime.h"

#include <memory>

namespace ATRX
{
	struct RenderPacket
	{
		DeltaTime DeltaTime;
	};

	enum class RendererBackendAPI
	{
		None,
		Vulkan,
		DirectX,
		OpenGL
	};

	class RendererBackend
	{
	public:
		static std::unique_ptr<RendererBackend> CreateInstance();

		virtual ~RendererBackend() = default;
		virtual bool OnInit() = 0;
		virtual void OnDestroy() = 0;

		virtual bool BeginFrame(DeltaTime dt) = 0;
		virtual bool EndFrame() = 0;

		inline static RendererBackendAPI GetBackendAPI() { return s_BackendAPI; }
	
	private:
		static RendererBackendAPI s_BackendAPI;
	};
}
