#include "glpch.h"
#include "VulkanInstance.h"

namespace Gale {

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		else
			return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
			func(instance, debugMessenger, pAllocator);
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		GL_INFO("Validation Layer: {}", pCallbackData->pMessage);
		
		return VK_FALSE;
	}

	VulkanInstance::VulkanInstance()
	{
		GetAvalibleExtensions();
		CreateInstance();
		SetupDebugMessenger();
	}

	VulkanInstance::~VulkanInstance()
	{
		if (EnableValidationLayers) 
			DestroyDebugUtilsMessengerEXT(s_Instance, DebugMessenger, nullptr);

		vkDestroyInstance(s_Instance, nullptr);
	}

	void VulkanInstance::CreateInstance()
	{
		if (EnableValidationLayers && !CheckValidationLayerSupport()) 
			GL_ERROR("Validation layers requested, but not available!");
		
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Vulkan";
		appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
		appInfo.apiVersion = VK_API_VERSION_1_2;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = GetRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (EnableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
			createInfo.ppEnabledLayerNames = ValidationLayers.data();

			debugCreateInfo = {};
			debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debugCreateInfo.pfnUserCallback = DebugCallback;

			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;

		}
		else 
			createInfo.enabledLayerCount = 0;

		if (vkCreateInstance(&createInfo, nullptr, &s_Instance) != VK_SUCCESS)
			GL_ERROR("Could not create instance");
		else
			GL_INFO("Vulkan instance created successfully!");
	}

	void VulkanInstance::GetAvalibleExtensions()
	{
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		GL_INFO("Avalible Vulkan Extensions:");

		for (const auto& extension : extensions)
		{
			GL_INFO("{}", extension.extensionName);
		}
	}


	std::vector<const char*> VulkanInstance::GetRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (EnableValidationLayers)
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		return extensions;
	}

	bool VulkanInstance::CheckValidationLayerSupport() 
	{
		uint32_t LayerCount;
		vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);
		std::vector<VkLayerProperties> AvailableLayers(LayerCount);
		vkEnumerateInstanceLayerProperties(&LayerCount, AvailableLayers.data());

		for (const char* LayerName : ValidationLayers) 
		{
			bool LayerFound = false;

			for (const auto& LayerProperties : AvailableLayers)
			{
				if (strcmp(LayerName, LayerProperties.layerName) == 0) 
				{
					LayerFound = true;
					break;
				}
			}

			if (!LayerFound)
			{
				return false;
			}
		}

		return true;
	}

	void VulkanInstance::SetupDebugMessenger()
	{
		if (!EnableValidationLayers) 
			return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = DebugCallback;

		if (CreateDebugUtilsMessengerEXT(s_Instance, &createInfo, nullptr, &DebugMessenger) != VK_SUCCESS)
			GL_ERROR("Failed to set up debug messenger!");
	}

}