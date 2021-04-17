#pragma once

#include "Gale/Core/Base.h"

#include "Platform/Windows/WindowsWindow.h"
#include "VulkanDevice.h"
#include "VulkanModel.h"
#include "VulkanPipeline.h"
#include "VulkanSwapChain.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace Gale
{
	struct SimplePushConstantData
	{
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};

	class VulkanContext
	{
	public:
		VulkanContext(Ref<WindowsWindow>);
		~VulkanContext();

		void drawFrame();
		void waitIdle();
		void onWindowResize(uint32_t width, uint32_t height);
	private:

		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain(uint32_t width, uint32_t height);
		void recordCommandBuffer(int imageIndex);

		GLFWwindow* m_WindowPtr;

		Ref<WindowsWindow> m_Window;
		Ref<VulkanDevice> m_Device;

		Scope<VulkanSwapChain> m_SwapChain;
		Scope<VulkanPipeline> m_Pipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		Scope<VulkanModel> m_Model;
	};
}