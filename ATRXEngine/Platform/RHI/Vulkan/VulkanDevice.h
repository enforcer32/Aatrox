#pragma once

#include "ATRXEngine/Renderer/API/RendererDevice.h"
#include "ATRXEngine/Platform/RHI/Vulkan/VulkanContext.h"
#include "ATRXEngine/Platform/RHI/Vulkan/VulkanPhysicalDevice.h"

namespace ATRX
{
	class VulkanDevice : public RendererDevice
	{
	public:
		virtual bool OnInit(const std::shared_ptr<RendererContext>& context) override;
		virtual void OnDestroy() override;

	private:
		bool InitLogicalDevice();
		void SetupQueueHandles();

	private:
		bool m_Initialized = false;
		std::shared_ptr<VulkanContext> m_Context;
		std::unique_ptr<VulkanPhysicalDevice> m_PhysicalDevice;
		VkDevice m_LogicalDevice;
		VkQueue m_GraphicsQueue;
		VkQueue m_ComputeQueue;
		VkQueue m_TransferQueue;
	};
}
