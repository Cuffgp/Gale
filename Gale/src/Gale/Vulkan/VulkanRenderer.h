#pragma once

#include "Gale/Renderer/RendererAPI.h"

#include "Gale/Vulkan/VulkanDevice.h"
#include "Gale/Vulkan/VulkanSwapchain.h"

namespace Gale {

	class VulkanRenderer : public RendererAPI
	{
	public:
		VulkanRenderer();
		~VulkanRenderer();

		VkCommandBuffer GetCurrentCommandBuffer() { return m_CommandBuffers[m_CurrentFrameIndex]; }

		virtual void BeginFrame() override;
		virtual void BeginSwapchainRendering(Ref<Pipeline> pipeline) override;
		virtual void EndSwapchainRendering() override;
		virtual void EndFrame() override;

		virtual void BindPipeline(Ref<Pipeline> pipeline) override;
		virtual void BindVertexBuffer(Ref<VertexBuffer> vertexBuffer) override;
		virtual void BindIndexBuffer(Ref<IndexBuffer> indexBuffer) override;
		virtual void BindUniformBuffer(Ref<UniformBuffer> uniformBuffer) override;
		virtual void BindDescriptorSet(Ref<DescriptorSet> descriptorSet) override;

		virtual void DrawIndexed(uint32_t indexCount) override;

		virtual void SetTransform(glm::mat4 transform) override;

		virtual uint32_t FramesInFlight() override { return MaxFramesInFlight; }
		virtual uint32_t CurrentFrameIndex() override { return m_CurrentFrameIndex; }

		virtual void WaitIdle() override { vkDeviceWaitIdle(VulkanDevice::Get().Device()); }
		virtual void OnWindowResize(uint32_t width, uint32_t height) override;
	public:
		static uint32_t MaxFramesInFlight;

	private:
		Ref<Pipeline> m_LatestPipeline;
		VkPipelineLayout m_LatestLayout;

		Scope<VulkanSwapchain> m_Swapchain;

		std::vector<VkCommandBuffer> m_CommandBuffers;

		PFN_vkCmdBeginRenderingKHR vkCmdBeginRenderingKHR;
		PFN_vkCmdEndRenderingKHR vkCmdEndRenderingKHR;

		uint32_t m_CurrentImageIndex = 0;
		uint32_t m_CurrentFrameIndex = 0;

		std::vector<VkSemaphore> m_ImageAvailableSemaphores;
		std::vector<VkSemaphore> m_DrawFinishedSemaphores;
		std::vector<VkFence> m_FrameInFlightFences;
		std::vector<VkFence> m_ImageInFlightFences;
	};


}