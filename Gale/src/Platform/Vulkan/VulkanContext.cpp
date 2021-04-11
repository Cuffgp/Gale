#include "glpch.h"
#include "VulkanContext.h"

namespace Gale {

	VulkanContext::VulkanContext(Ref<WindowsWindow> window) : m_Window(window)
	{
		VkExtent2D extent = { static_cast<uint32_t>(m_Window->GetWidth()), static_cast<uint32_t>(m_Window->GetHeight()) };

		m_Device = CreateRef<VulkanDevice>(m_Window->GetWindowPointer());
		m_SwapChain = CreateRef<VulkanSwapChain>(m_Device, extent);

		createPipelineLayout();
		createPipeline();
		createCommandBuffers();
	}

	VulkanContext::~VulkanContext()
	{
		vkDestroyPipelineLayout(m_Device->device(), pipelineLayout, nullptr);
	}

	void VulkanContext::createPipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(m_Device->device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
			VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}

	void VulkanContext::createPipeline() 
	{
		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::defaultPipelineConfigInfo(
			pipelineConfig,
			m_SwapChain->width(),
			m_SwapChain->height());
		pipelineConfig.renderPass = m_SwapChain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		m_Pipeline = CreateScope<VulkanPipeline>(
			m_Device,
			"assets/vert.spv",
			"assets/frag.spv",
			pipelineConfig);
	}

	void VulkanContext::createCommandBuffers() 
	{
		commandBuffers.resize(m_SwapChain->imageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_Device->getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(m_Device->device(), &allocInfo, commandBuffers.data()) !=
			VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}

		for (int i = 0; i < commandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("failed to begin recording command buffer!");
			}

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = m_SwapChain->getRenderPass();
			renderPassInfo.framebuffer = m_SwapChain->getFrameBuffer(i);

			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = m_SwapChain->getSwapChainExtent();

			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
			clearValues[1].depthStencil = { 1.0f, 0 };
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			m_Pipeline->bind(commandBuffers[i]);
			vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(commandBuffers[i]);
			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer!");
			}
		}
	}

	void VulkanContext::drawFrame() {
		uint32_t imageIndex;
		auto result = m_SwapChain->acquireNextImage(&imageIndex);
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		result = m_SwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}
	}

	void VulkanContext::waitIdle()
	{
		vkDeviceWaitIdle(m_Device->device());
	}

}