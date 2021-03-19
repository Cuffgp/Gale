#pragma once

#include "vulkan/vulkan.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Sleet {

	// Creates the Vulkan Instance and set up Vlaidation Layers
	class VulkanInstance
	{
	public:
		VulkanInstance();
		~VulkanInstance();

		static VkInstance GetInstance() { return s_Instance; }

	public:
		const static bool EnableValidationLayers = true;
	private:
		void CreateInstance();
		void GetAvalibleExtensions();
		std::vector<const char*> GetRequiredExtensions();
		bool CheckValidationLayerSupport();
		void SetupDebugMessenger();

	private:
		inline static VkInstance s_Instance;
		VkDebugUtilsMessengerEXT DebugMessenger;
		const std::vector<const char*> ValidationLayers = { "VK_LAYER_KHRONOS_validation" };
	};

}
