#include "ATRXEngine/Renderer/Vulkan/VulkanContext.h"
#include "ATRXEngine/Core/Logger.h"
#include "ATRXEngine/Core/Platform.h"

#include <sstream>

namespace ATRX
{
	VKAPI_ATTR VkBool32 VKAPI_CALL VkDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData)
	{
		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			ATRX_LOG_TRACE(callbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			ATRX_LOG_INFO(callbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			ATRX_LOG_WARN(callbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			ATRX_LOG_INFO(callbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
			break;
		default:
			break;
		}

		return VK_FALSE;
	}

	bool VulkanContext::OnInit()
	{
		ATRX_LOG_INFO("ATRXVulkanContext->Initializing...");

		VkApplicationInfo appInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
		appInfo.apiVersion = VK_API_VERSION_1_2;
		appInfo.pApplicationName = "Aatrox";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Aatrox";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

		if (!InitVulkanExtensions())
		{
			ATRX_LOG_ERROR("ATRXVulkanContext->Error InitVulkanExtensions");
			return false;
		}

		if (!InitVulkanValidationLayers())
		{
			ATRX_LOG_ERROR("ATRXVulkanContext->Error InitVulkanValidationLayers");
			return false;
		}

		VkInstanceCreateInfo instanceInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
		instanceInfo.pApplicationInfo = &appInfo;
		instanceInfo.enabledExtensionCount = m_VulkanExtensions.size();
		instanceInfo.ppEnabledExtensionNames = m_VulkanExtensions.data();
		instanceInfo.enabledLayerCount = m_VulkanValidationLayers.size();
		instanceInfo.ppEnabledLayerNames = m_VulkanValidationLayers.data();

		VkResult res = vkCreateInstance(&instanceInfo, m_Allocator, &m_Instance);
		if (res != VK_SUCCESS)
		{
			ATRX_LOG_ERROR("ATRXVulkanContext->Error vkCreateInstance: ({})!", (int)res);
			return false;
		}

		// Vulkan Debugger
		if (m_ValidationLayerEnabled)
		{
			if (!InitVulkanDebugger())
			{
				ATRX_LOG_ERROR("ATRXVulkanContext->Error InitVulkanDebugger!");
				return false;
			}
		}

		ATRX_LOG_INFO("ATRXVulkanContext->Initialized!");
		return m_Initialized = true;
	}

	void VulkanContext::OnDestroy()
	{
		if (m_Initialized)
		{
			ATRX_LOG_INFO("ATRXVulkanContext->Destroying...");

			if (m_DebugMessenger)
			{
				ATRX_LOG_INFO("ATRXVulkanContext->Destroying Debugger...");
				PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugUtilsMessengerEXT");
				if (!func)
					ATRX_LOG_ERROR("ATRXVulkanContext->Failed to Retrieve Function Pointer: (vkDestroyDebugUtilsMessengerEXT)");
				else
					func(m_Instance, m_DebugMessenger, m_Allocator);
				ATRX_LOG_INFO("ATRXVulkanContext->Destroyed Debugger!");
			}

			vkDestroyInstance(m_Instance, m_Allocator);

			ATRX_LOG_INFO("ATRXVulkanContext->Destroyed!");
			m_Initialized = false;
		}
	}

	bool VulkanContext::InitVulkanExtensions()
	{
		ATRX_LOG_INFO("ATRXVulkanContext->Initializing Vulkan Extensions...");
		// Vulkan Extensions
		EnableVulkanExtension(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef ATRX_ENGINE_DEBUG
		EnableVulkanExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif // ATRX_ENGINE_DEBUG
#ifdef ATRX_PLATFORM_WINDOWS
		EnableVulkanExtension("VK_KHR_win32_surface");
#endif // ATRX_PLATFORM_WINDOWS
		ATRX_LOG_INFO("ATRXVulkanContext->Initialized Vulkan Extensions!");
		return true;
	}

	bool VulkanContext::InitVulkanValidationLayers()
	{
#ifdef ATRX_ENGINE_DEBUG
		ATRX_LOG_INFO("ATRXVulkanContext->Initializing Vulkan Validation Layers...");
		EnableVulkanValidationLayer("VK_LAYER_KHRONOS_validation");

		ATRX_LOG_DEBUG("ATRXVulkanContext->Enumerating Validation Layers...");

		uint32_t availableLayerCount = 0;
		VkResult res = vkEnumerateInstanceLayerProperties(&availableLayerCount, 0);
		if (res != VK_SUCCESS)
		{
			ATRX_LOG_ERROR("ATRXVulkanContext->Error vkEnumerateInstanceLayerProperties: ({})!", (int)res);
			return false;
		}

		VkLayerProperties* availableLayers = new VkLayerProperties[availableLayerCount];
		res = vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers);
		if (res != VK_SUCCESS)
		{
			ATRX_LOG_ERROR("ATRXVulkanContext->Error vkEnumerateInstanceLayerProperties: ({})!", (int)res);
		}

		for (size_t i = 0; i < m_VulkanValidationLayers.size(); i++)
		{
			ATRX_LOG_INFO("ATRXVulkanContext->Searching Vulkan Validation Layer: ({})", m_VulkanValidationLayers[i]);
			bool flag = false;
			for (size_t j = 0; j < availableLayerCount; j++)
			{
				if (!std::strcmp(m_VulkanValidationLayers[i], availableLayers[j].layerName))
				{
					flag = true;
					ATRX_LOG_INFO("ATRXVulkanContext->Found Vulkan Validation Layer: ({})!", m_VulkanValidationLayers[i]);
					break;
				}
			}

			if (!flag)
			{
				ATRX_LOG_ERROR("ATRXVulkanContext->Missing Vulkan Validation Layer: ({})", m_VulkanValidationLayers[i]);
				return false;
			}
		}

		m_ValidationLayerEnabled = true;
		ATRX_LOG_INFO("ATRXVulkanContext->Initialized Vulkan Validation Layers!");
#endif // ATRX_ENGINE_DEBUG
		return true;
	}

	bool VulkanContext::InitVulkanDebugger()
	{
		ATRX_LOG_INFO("ATRXVulkanContext->Initializing Vulkan Debugger...");
		uint32_t logSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;

		VkDebugUtilsMessengerCreateInfoEXT debugInfo = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
		debugInfo.messageSeverity = logSeverity;
		debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
		debugInfo.pfnUserCallback = VkDebugCallback;

		PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkCreateDebugUtilsMessengerEXT");
		if (!func)
		{
			ATRX_LOG_ERROR("ATRXVulkanContext->Failed to Retrieve Function Pointer: (vkCreateDebugUtilsMessengerEXT)");
			return false;
		}

		VkResult res = func(m_Instance, &debugInfo, m_Allocator, &m_DebugMessenger);
		if (res != VK_SUCCESS)
		{
			ATRX_LOG_ERROR("ATRXVulkanContext->Error vkCreateDebugUtilsMessengerEXT: ({})!", (int)res);
			return false;
		}

		ATRX_LOG_INFO("ATRXVulkanContext->Initialized Vulkan Debugger!");
		return true;
	}

	void VulkanContext::EnableVulkanExtension(const char* extension)
	{
		m_VulkanExtensions.push_back(extension);
		ATRX_LOG_INFO("ATRXVulkanContext->Enabling Vulkan Extension: ({})", extension);
	}

	void VulkanContext::EnableVulkanValidationLayer(const char* validationLayer)
	{
		m_VulkanValidationLayers.push_back(validationLayer);
		ATRX_LOG_INFO("ATRXVulkanContext->Enabling Vulkan Validation Layer: ({})", validationLayer);
	}
}
