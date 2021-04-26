#include "glpch.h"
#include "VulkanContext.h"

namespace Gale {

	VulkanContext::VulkanContext(Ref<WindowsWindow> window) : m_Window(window)
	{
		m_Device = CreateRef<VulkanDevice>(m_Window->GetWindowPointer());
		loadModels();
		createDescriptorSetLayout();
		createPipelineLayout();
		recreateSwapChain(static_cast<uint32_t>(m_Window->GetWidth()), static_cast<uint32_t>(m_Window->GetHeight()));
		createCommandBuffers();
		createUniformBuffers();
		createDescriptorPool();
		createDescriptorSet();
	}

	VulkanContext::~VulkanContext()
	{
		for (size_t i = 0; i < m_SwapChain->imageCount(); i++)
		{
			vkDestroyBuffer(m_Device->device(), uniformBuffers[i], nullptr);
			vkFreeMemory(m_Device->device(), uniformBuffersMemory[i], nullptr);
		}

		vkDestroyDescriptorPool(m_Device->device(), descriptorPool, nullptr);

		vkDestroyDescriptorSetLayout(m_Device->device(), descriptorSetLayout, nullptr);
		vkDestroyPipelineLayout(m_Device->device(), pipelineLayout, nullptr);
	}

	void VulkanContext::loadModels()
	{
		std::vector<VulkanModel::Vertex> vertices
		{
			{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
			{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
			{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
		};

		std::vector<uint32_t> indices
		{0, 1, 2, 2, 3, 0};

		//m_Model = CreateScope<VulkanModel>(m_Device, vertices, indices);
		m_Model = CreateScope<VulkanModel>(m_Device, "assets/objects/Spot.obj");
		m_Texture = CreateRef<VulkanTexture>(m_Device, "assets/textures/spot_texture.png");
	}

	void VulkanContext::createDescriptorSetLayout()
	{
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

		VkDescriptorSetLayoutBinding samplerLayoutBinding = m_Texture->getDescriptor();

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		if (vkCreateDescriptorSetLayout(m_Device->device(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
			GL_ERROR("failed to create descriptor set layout!");

	}

	void VulkanContext::createUniformBuffers()
	{
		VkDeviceSize bufferSize = sizeof(UniformBufferObject);

		uniformBuffers.resize(m_SwapChain->imageCount());
		uniformBuffersMemory.resize(m_SwapChain->imageCount());

		for (size_t i = 0; i < m_SwapChain->imageCount(); i++)
		{
			m_Device->createBuffer(bufferSize,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				uniformBuffers[i],
				uniformBuffersMemory[i]);
		}
	}

	void VulkanContext::createDescriptorPool()
	{
		std::array<VkDescriptorPoolSize, 2> poolSizes{};
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(m_SwapChain->imageCount());
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(m_SwapChain->imageCount());

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(m_SwapChain->imageCount());

		if (vkCreateDescriptorPool(m_Device->device(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
			GL_ERROR("failed to create descriptor pool!");
	}

	void VulkanContext::createDescriptorSet()
	{
		std::vector<VkDescriptorSetLayout> layouts(m_SwapChain->imageCount(), descriptorSetLayout);

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(m_SwapChain->imageCount());
		allocInfo.pSetLayouts = layouts.data();

		descriptorSets.resize(m_SwapChain->imageCount());
		if (vkAllocateDescriptorSets(m_Device->device(), &allocInfo, descriptorSets.data()) != VK_SUCCESS)
			GL_ERROR("failed to allocate descriptor sets!");

		for (size_t i = 0; i < m_SwapChain->imageCount(); i++)
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = uniformBuffers[i];
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = m_Texture->getView();
			imageInfo.sampler = m_Texture->getSampler();

			std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = descriptorSets[i];
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &bufferInfo;

			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = descriptorSets[i];
			descriptorWrites[1].dstBinding = 1;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(m_Device->device(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}

	}

	void VulkanContext::updateUniformBuffer(uint32_t imageIndex)
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		UniformBufferObject ubo{};
		ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		ubo.view = glm::lookAt(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		ubo.proj = glm::perspective(glm::radians(45.0f), (float)m_SwapChain->width() / (float)m_SwapChain->height(), 0.1f, 10.0f);
		ubo.proj[1][1] *= -1;

		void* data;
		vkMapMemory(m_Device->device(), uniformBuffersMemory[imageIndex], 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(m_Device->device(), uniformBuffersMemory[imageIndex]);
	}

	void VulkanContext::createPipelineLayout()
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
		//pipelineLayoutInfo.setLayoutCount = 0;
		//pipelineLayoutInfo.pSetLayouts = nullptr;

		//pipelineLayoutInfo.pushConstantRangeCount = 1;
		//pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
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
			"assets/shaders/shader.vert.spv",
			"assets/shaders/shader.frag.spv",
			pipelineConfig);
	}

	void VulkanContext::recreateSwapChain(uint32_t width, uint32_t height)
	{
		VkExtent2D extent = { width, height };

		vkDeviceWaitIdle(m_Device->device());

		if (m_SwapChain == nullptr)
			m_SwapChain = CreateRef<VulkanSwapChain>(m_Device, extent);
		else
		{
			m_SwapChain = CreateRef<VulkanSwapChain>(m_Device, extent, std::move(m_SwapChain));
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

		float time = m_Window->GetTime();

		/*
		SimplePushConstantData push{};
		//push.projection = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
		//push.view = glm::rotate(glm::mat4(1.0f), time, glm::vec3(0.0f, 1.0f, 0.0f));
		
		vkCmdPushConstants(
			commandBuffers[imageIndex],
			pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(SimplePushConstantData),
			&push);
		*/


		vkCmdBindDescriptorSets(commandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipelineLayout, 0, 1, &descriptorSets[imageIndex], 0, nullptr);

		m_Model->draw(commandBuffers[imageIndex]);

		vkCmdEndRenderPass(commandBuffers[imageIndex]);
		if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
			GL_ERROR("failed to record command buffer!");
	}

	void VulkanContext::drawFrame()
	{

		uint32_t imageIndex;
		auto result = m_SwapChain->acquireNextImage(&imageIndex);

		
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain(static_cast<uint32_t>(m_Window->GetWidth()), static_cast<uint32_t>(m_Window->GetHeight()));
			return;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}
		
		

		updateUniformBuffer(imageIndex);

		recordCommandBuffer(imageIndex);
		result = m_SwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

		
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			recreateSwapChain(static_cast<uint32_t>(m_Window->GetWidth()), static_cast<uint32_t>(m_Window->GetHeight()));
			return;
		}
		

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