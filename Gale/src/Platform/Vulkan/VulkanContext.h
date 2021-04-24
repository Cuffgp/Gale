#pragma once

#include "Gale/Core/Base.h"

#include "Platform/Windows/WindowsWindow.h"
#include "VulkanDevice.h"
#include "VulkanPipeline.h"
#include "VulkanSwapChain.h"
#include "VulkanModel.h"
#include "VulkanTexture.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Gale
{

	struct UniformBufferObject
	{
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);
	};

	struct SimplePushConstantData
	{
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
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

		Scope<VulkanModel> m_Model;
		Ref<VulkanTexture> m_Texture;

		Ref<WindowsWindow> m_Window;
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapChain> m_SwapChain;
		Scope<VulkanPipeline> m_Pipeline;

		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;

		// Will probably abstract to its own class
		VkDescriptorSetLayout descriptorSetLayout;
		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;

		void createUniformBuffers();
		void createDescriptorSetLayout();
		void createDescriptorPool();
		void createDescriptorSet();
		void updateUniformBuffer(uint32_t imageIndex);

	};
}