#include "ATRXEngine/Platform/RHI/Vulkan/VulkanImage.h"
#include "ATRXEngine/Core/Logger.h"

namespace ATRX
{
	bool VulkanImage::OnInit(const std::shared_ptr<RendererContext>& context, const std::shared_ptr<RendererDevice>& device, const RendererImageProperties& imageProperties)
	{
		ATRX_LOG_INFO("ATRXVulkanImage->Initializing...");
		m_Context = std::dynamic_pointer_cast<VulkanContext>(context);
		m_Device = std::dynamic_pointer_cast<VulkanDevice>(device);
		m_ImageProperties = imageProperties;

		if (!CreateImage(true))
		{
			ATRX_LOG_ERROR("ATRXVulkanImage->Error CreateImage!");
			return false;
		}

		ATRX_LOG_INFO("ATRXVulkanImage->Initialized!");
		return m_Initialized = true;
	}

	void VulkanImage::OnDestroy()
	{
		if (m_Initialized)
		{
			ATRX_LOG_INFO("ATRXVulkanImage->Destroying...");
			if (m_ImageView)
				vkDestroyImageView(m_Device->GetInternalDevice(), m_ImageView, m_Context->GetAllocator());
			if (m_ImageMemory)
				vkFreeMemory(m_Device->GetInternalDevice(), m_ImageMemory, m_Context->GetAllocator());
			if (m_Image)
				vkDestroyImage(m_Device->GetInternalDevice(), m_Image, m_Context->GetAllocator());
			ATRX_LOG_INFO("ATRXVulkanImage->Destroyed!");
			m_Initialized = false;
		}
	}

	bool VulkanImage::CreateImage(bool createImageView)
	{
		ATRX_LOG_INFO("ATRXVulkanImage->Creating Image...");
		// Create Image
		VkImageCreateInfo imageCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D; // CONFIG?
		imageCreateInfo.extent.width = m_ImageProperties.Width;
		imageCreateInfo.extent.height = m_ImageProperties.Height;
		imageCreateInfo.extent.depth = m_ImageProperties.Depth;
		imageCreateInfo.mipLevels = m_ImageProperties.Mips;
		imageCreateInfo.arrayLayers = m_ImageProperties.Layers;
		imageCreateInfo.format = (VkFormat)m_ImageProperties.Format;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.usage = m_ImageProperties.Usage;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkResult res = vkCreateImage(m_Device->GetInternalDevice(), &imageCreateInfo, m_Context->GetAllocator(), &m_Image);
		if (res != VK_SUCCESS)
		{
			ATRX_LOG_ERROR("ATRXVulkanImage->Error vkCreateImage: ({})!", (int)res);
			return false;
		}
		
		// Allocate Memory
		VkMemoryRequirements memoryReq;
		vkGetImageMemoryRequirements(m_Device->GetInternalDevice(), m_Image, &memoryReq);

		int32_t memoryTypeIndex = m_Device->GetPhysicalDevice()->GetMemoryTypeIndex(memoryReq.memoryTypeBits, m_ImageProperties.MemoryType);
		if (memoryTypeIndex == -1)
		{
			ATRX_LOG_ERROR("ATRXVulkanImage->Error GetMemoryTypeIndex: ({})!", (int)memoryTypeIndex);
			return false;
		}

		// Write Custom Vulkan Allocator?
		VkMemoryAllocateInfo memoryAllocateInfo = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
		memoryAllocateInfo.allocationSize = memoryReq.size;
		memoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;

		res = vkAllocateMemory(m_Device->GetInternalDevice(), &memoryAllocateInfo, m_Context->GetAllocator(), &m_ImageMemory);
		if (res != VK_SUCCESS)
		{
			ATRX_LOG_ERROR("ATRXVulkanImage->Error vkAllocateMemory: ({})!", (int)res);
			return false;
		}

		res = vkBindImageMemory(m_Device->GetInternalDevice(), m_Image, m_ImageMemory, 0);
		if (res != VK_SUCCESS)
		{
			ATRX_LOG_ERROR("ATRXVulkanImage->Error vkBindImageMemory: ({})!", (int)res);
			return false;
		}

		if (createImageView)
		{
			if (!CreateImageView())
			{
				ATRX_LOG_ERROR("ATRXVulkanImage->Error CreateImageView!");
				return false;
			}
		}

		ATRX_LOG_INFO("ATRXVulkanImage->Created Image!");
		return true;
	}

	bool VulkanImage::CreateImageView()
	{
		ATRX_LOG_INFO("ATRXVulkanImage->Creating ImageView...");
		VkImageViewCreateInfo imageViewCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
		imageViewCreateInfo.image = m_Image;
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = (VkFormat)m_ImageProperties.Format;
		imageViewCreateInfo.subresourceRange.aspectMask = m_ImageProperties.AspectFlags;
		imageViewCreateInfo.subresourceRange.layerCount = m_ImageProperties.Layers;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;

		VkResult res = vkCreateImageView(m_Device->GetInternalDevice(), &imageViewCreateInfo, m_Context->GetAllocator(), &m_ImageView);
		if (res != VK_SUCCESS)
		{
			ATRX_LOG_ERROR("ATRXVulkanImage->Error vkCreateImageView: ({})!", (int)res);
			return false;
		}

		ATRX_LOG_INFO("ATRXVulkanImage->Created ImageView!");
		return true;
	}
}
