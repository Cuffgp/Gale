#include "glpch.h"
#include "VulkanContext.h"

namespace Gale {

	VulkanContext::VulkanContext(Ref<WindowsWindow> window) : m_Window(window)
	{

		m_Device = CreateRef<VulkanDevice>(m_Window->GetWindowPointer());

		loadModels();
		createPipelineLayout();
		recreateSwapChain(static_cast<uint32_t>(m_Window->GetWidth()), static_cast<uint32_t>(m_Window->GetHeight()));
		createCommandBuffers();
	}

	VulkanContext::~VulkanContext()
	{
		vkDestroyPipelineLayout(m_Device->device(), pipelineLayout, nullptr);
	}

	void VulkanContext::loadModels()
	{
		std::vector<VulkanModel::Vertex> vertices
		{
			{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
		};

		std::vector<uint32_t> indices
		{0, 1, 2, 2, 3, 0};

		m_Model = CreateScope<VulkanModel>(m_Device, vertices, indices);
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
		assert(m_SwapChain != nullptr && "Cannot create pipeline before swap chain");
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = m_SwapChain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		m_Pipeline = CreateScope<VulkanPipeline>(
			m_Device,
			"assets/vert.spv",
			"assets/frag.spv",
			pipelineConfig);
	}

	void VulkanContext::recreateSwapChain(uint32_t width, uint32_t height)
	{
		VkExtent2D extent = { width, height };

		vkDeviceWaitIdle(m_Device->device());

		if (m_SwapChain == nullptr)
			m_SwapChain = CreateScope<VulkanSwapChain>(m_Device, extent);
		else
		{
			m_SwapChain = CreateScope<VulkanSwapChain>(m_Device, extent, std::move(m_SwapChain));
			if (m_SwapChain->imageCount() != commandBuffers.size())
			{
				freeCommandBuffers();
				createCommandBuffers();
			}
		}

		createPipeline();
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

	}

	void VulkanContext::freeCommandBuffers()
	{
		vkFreeCommandBuffers(m_Device->device(), m_Device->getCommandPool(),
			static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
		commandBuffers.clear();
	}

	void VulkanContext::recordCommandBuffer(int imageIndex)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_SwapChain->getRenderPass();
		renderPassInfo.framebuffer = m_SwapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_SwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_SwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(m_SwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, m_SwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

		m_Pipeline->bind(commandBuffers[imageIndex]);

		m_Model->bind(commandBuffers[imageIndex]);
		m_Model->draw(commandBuffers[imageIndex]);

		vkCmdEndRenderPass(commandBuffers[imageIndex]);
		if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}

	void VulkanContext::drawFrame() {
		uint32_t imageIndex;
		auto result = m_SwapChain->acquireNextImage(&imageIndex);

		/*
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain(static_cast<uint32_t>(m_Window->GetWidth()), static_cast<uint32_t>(m_Window->GetHeight()));
			return;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}
		*/

		recordCommandBuffer(imageIndex);
		result = m_SwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

		/*
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			recreateSwapChain(static_cast<uint32_t>(m_Window->GetWidth()), static_cast<uint32_t>(m_Window->GetHeight()));
			return;
		}
		*/

		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}
	}

	void VulkanContext::waitIdle()
	{
		vkDeviceWaitIdle(m_Device->device());
	}

	void VulkanContext::onWindowResize(uint32_t width, uint32_t height)
	{
		recreateSwapChain(width, height);
		drawFrame();
	}

}