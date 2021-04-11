#pragma once

#include "Gale/Core/Base.h"

#include "Platform/Windows/WindowsWindow.h"
#include "VulkanDevice.h"
#include "VulkanModel.h"
#include "VulkanPipeline.h"
#include "VulkanSwapChain.h"

namespace Gale
{
	class VulkanContext
	{
	public:
		VulkanContext(Ref<WindowsWindow>);
		~VulkanContext();

		void drawFrame();
		void waitIdle();
	private:

		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();

		GLFWwindow* m_WindowPtr;

		Ref<WindowsWindow> m_Window;
		Ref<VulkanDevice> m_Device;

		Ref<VulkanSwapChain> m_SwapChain;
		Scope<VulkanPipeline> m_Pipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		Scope<VulkanModel> m_Model;
	};
}