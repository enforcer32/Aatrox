#pragma once

#include "ATRXEngine/Renderer/API/RendererBackend.h"
#include "ATRXEngine/Platform/RHI/Vulkan/VulkanContext.h"

namespace ATRX
{
	class VulkanRenderer : public RendererBackend
	{
	public:
		virtual bool OnInit() override;
		virtual void OnDestroy() override;

		virtual bool BeginFrame(DeltaTime dt) override;
		virtual bool EndFrame() override;

	private:
		bool m_Initialized = false;
		std::shared_ptr<VulkanContext> m_Context;
	};
}
