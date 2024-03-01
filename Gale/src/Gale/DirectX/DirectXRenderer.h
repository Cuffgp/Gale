#pragma once

#include "Gale/Renderer/RendererAPI.h"

#include "Gale/DirectX/DirectXDevice.h"
#include "Gale/DirectX/DirectXSwapchain.h"
#include "Gale/DirectX/DirectXPipeline.h"

namespace Gale {

	class DirectXRenderer : public RendererAPI
	{
	public:
		DirectXRenderer();
		~DirectXRenderer();

		ID3D12GraphicsCommandList* GetCommandList() { return m_CommandList; }

		virtual void BeginFrame() override; 
		virtual void BeginSwapchainRendering(Ref<Pipeline> pipeline) override;
		virtual void EndSwapchainRendering() override;
		virtual void EndFrame() override;

		virtual void WaitIdle() override { }
		virtual void OnWindowResize(uint32_t width, uint32_t height) override {};

		virtual void BindPipeline(Ref<Pipeline> pipeline) override;
		virtual void BindVertexBuffer(Ref<VertexBuffer> vertexBuffer) override;
		virtual void BindIndexBuffer(Ref<IndexBuffer> indexBuffer) override;
		virtual void BindUniformBuffer(Ref<UniformBuffer> uniformBuffer) override;
		virtual void BindDescriptorSet(Ref<DescriptorSet> descriptorSet) override;

		virtual void DrawIndexed(uint32_t indexCount) override;

		virtual void SetTransform(glm::mat4 transform) override;

		virtual uint32_t FramesInFlight() override { return MaxFramesInFlight; }
		virtual uint32_t CurrentFrameIndex() override { return m_CurrentFrameIndex; }

		void WaitForGPU();
		void MoveToNextFrame();

		const static uint32_t MaxFramesInFlight = 2;
	private:
		Scope<DirectXSwapchain> m_Swapchain;

		uint32_t m_CurrentFrameIndex = 0;
		HANDLE m_FenceEvent;
		ID3D12Fence* m_Fence;

		UINT64 m_FenceValues[MaxFramesInFlight] = {};

		ID3D12CommandAllocator* m_CommandAllocators[MaxFramesInFlight];
		ID3D12GraphicsCommandList* m_CommandList;
	};

}