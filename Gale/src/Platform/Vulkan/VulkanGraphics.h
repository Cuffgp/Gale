#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Vulkan.h"

#include <optional>
#include <set>

namespace Gale {

	struct QueueFamilyIndices 
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() 
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails 
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	// One big class for all vulkan code.
	// get this working then work on abstarction.

	class VulkanGraphics
	{
	public:
		VulkanGraphics(GLFWwindow* window);
		~VulkanGraphics();

		void createInstance();
		std::vector<const char*> getRequiredExtensions();

		void pickPhysicalDevice();
		bool isDeviceSuitable(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		QueueFamilyIndices VulkanGraphics::findQueueFamilies(VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

		void createLogicalDevice();

		void createSwapChain();
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void recreateSwapChain();
		void cleanupSwapChain();

		void createImageViews();
		void createRenderPass();
		void createGraphicsPipeline();
		VkShaderModule createShaderModule(const std::vector<char>& code);
		void createFramebuffers();
		void createCommandPool();
		void createCommandBuffers();

		void createSyncObjects();

		void drawFrame();

		void end();

	private:

		GLFWwindow* m_window;

		VkInstance m_instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		const bool enableValidationLayers = true;
		const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

		VkSurfaceKHR m_surface;

		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
		VkDevice m_device;
		const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		VkQueue graphicsQueue;
		VkQueue presentQueue;

		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;

		std::vector<VkImageView> swapChainImageViews;

		VkRenderPass renderPass;

		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;

		std::vector<VkFramebuffer> swapChainFramebuffers;

		VkCommandPool commandPool;

		std::vector<VkCommandBuffer> commandBuffers;

		const int max_frames_in_flight = 2;
		size_t currentFrame = 0;
		

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		std::vector<VkFence> imagesInFlight;

	public:
		bool framebufferResized = false;
	};

}