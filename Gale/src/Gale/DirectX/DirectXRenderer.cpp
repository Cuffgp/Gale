#include "glpch.h"

#include "Gale/DirectX/DirectXRenderer.h"
#include "Gale/DirectX/DirectXPipeline.h"

#include "Gale/DirectX/DirectXVertexBuffer.h"
#include "Gale/DirectX/DirectXIndexBuffer.h"
#include "Gale/DirectX/DirectXUniformBuffer.h"
#include "Gale/DirectX/DirectXDescriptorSet.h"

namespace Gale {

	DirectXRenderer::DirectXRenderer()
	{
		m_Swapchain = CreateScope<DirectXSwapchain>();

		auto device = DirectXDevice::Get().Device();

		for (int i = 0; i < MaxFramesInFlight; i++)
		{
			device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocators[i]));
		}
		device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocators[0], nullptr, IID_PPV_ARGS(&m_CommandList));
		m_CommandList->Close();
		device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));
		m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

		//WaitForGPU();
	}

	DirectXRenderer::~DirectXRenderer()
	{
		WaitForGPU();
		CloseHandle(m_FenceEvent);

		for (int i = 0; i < MaxFramesInFlight; i++)
		{
			m_CommandAllocators[i]->Release();
		}
		m_CommandList->Release();
		
	}

	void DirectXRenderer::BeginFrame()
	{
		auto result = m_CommandAllocators[m_CurrentFrameIndex]->Reset();
		if (result != S_OK)
		{
			GL_ERROR("Failed to reset command allocator");
		}

		result = m_CommandList->Reset(m_CommandAllocators[m_CurrentFrameIndex], nullptr);
		if (result != S_OK)
		{
			GL_ERROR("Failed to reset command list");
		}
	}

	void DirectXRenderer::BeginSwapchainRendering(Ref<Pipeline> pipeline)
	{
		D3D12_RECT surfaceSize;
		surfaceSize.left = 0;
		surfaceSize.top = 0;
		surfaceSize.right = static_cast<LONG>(m_Swapchain->GetWidth());
		surfaceSize.bottom = static_cast<LONG>(m_Swapchain->GetHeight());

		D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = static_cast<float>(m_Swapchain->GetWidth());
		viewport.Height = static_cast<float>(m_Swapchain->GetHeight());
		viewport.MinDepth = .1f;
		viewport.MaxDepth = 1.f;

		m_CommandList->SetGraphicsRootSignature(DirectXPipeline::GetRootSignature());
		m_CommandList->RSSetViewports(1, &viewport);
		m_CommandList->RSSetScissorRects(1, &surfaceSize);

		m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_Swapchain->GetRenderTarget(m_CurrentFrameIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_Swapchain->GetRTVHeap()->GetCPUDescriptorHandleForHeapStart());
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_Swapchain->GetDSVHeap()->GetCPUDescriptorHandleForHeapStart());

		rtvHandle.ptr = rtvHandle.ptr + (m_CurrentFrameIndex * m_Swapchain->GetRTVDescriptorSize());
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
		m_CommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, NULL);

		const float clearColor[] = { 0.7f, 0.1f, 0.1f, 1.0f };
		m_CommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void DirectXRenderer::EndSwapchainRendering()
	{
		m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_Swapchain->GetRenderTarget(m_CurrentFrameIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
		m_CommandList->Close();
	}

	void DirectXRenderer::EndFrame()
	{
		auto commandQueue = DirectXDevice::Get().CommandQueue();

		// Execute the command list.
		ID3D12CommandList* ppCommandLists[] = { m_CommandList };
		commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		// Present the frame.
		m_Swapchain->Present(1, 0);

		MoveToNextFrame();
	}

	void DirectXRenderer::BindPipeline(Ref<Pipeline> pipeline)
	{
		auto state = std::static_pointer_cast<DirectXPipeline>(pipeline)->GetState();
		m_CommandList->SetPipelineState(state);
	}

	void DirectXRenderer::BindVertexBuffer(Ref<VertexBuffer> vertexBuffer)
	{
		auto view = std::static_pointer_cast<DirectXVertexBuffer>(vertexBuffer)->GetView();
		m_CommandList->IASetVertexBuffers(0, 1, &view);
	}

	void DirectXRenderer::BindIndexBuffer(Ref<IndexBuffer> indexBuffer)
	{
		auto view = std::static_pointer_cast<DirectXIndexBuffer>(indexBuffer)->GetView();
		m_CommandList->IASetIndexBuffer(&view);
	}

	void DirectXRenderer::BindUniformBuffer(Ref<UniformBuffer> uniformBuffer)
	{
		auto address = std::static_pointer_cast<DirectXUniformBuffer>(uniformBuffer)->GetGPUAddress();
		m_CommandList->SetGraphicsRootConstantBufferView(1, address);
	}

	void DirectXRenderer::BindDescriptorSet(Ref<DescriptorSet> descriptorSet)
	{
		auto heap = std::static_pointer_cast<DirectXDescriptorSet>(descriptorSet)->GetUniformHeap();

		ID3D12DescriptorHeap* ppHeaps[] = { heap };
		m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

		m_CommandList->SetGraphicsRootDescriptorTable(1, heap->GetGPUDescriptorHandleForHeapStart());
	}

	void DirectXRenderer::DrawIndexed(uint32_t indexCount)
	{
		m_CommandList->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
	}

	void DirectXRenderer::SetTransform(glm::mat4 transform)
	{
		// Hard coded for now but we want to eventually reflect this from the shader
		m_CommandList->SetGraphicsRoot32BitConstants(0, 16, &transform, 0);
	}

	void DirectXRenderer::WaitForGPU()
	{
		auto commandQueue = DirectXDevice::Get().CommandQueue();
		// Schedule a Signal command in the queue.
		commandQueue->Signal(m_Fence, m_FenceValues[m_CurrentFrameIndex]);

		// Wait until the fence has been processed.
		m_Fence->SetEventOnCompletion(m_FenceValues[m_CurrentFrameIndex], m_FenceEvent);
		WaitForSingleObjectEx(m_FenceEvent, INFINITE, FALSE);

		// Increment the fence value for the current frame.
		m_FenceValues[m_CurrentFrameIndex]++;
	}

	void DirectXRenderer::MoveToNextFrame()
	{
		auto commandQueue = DirectXDevice::Get().CommandQueue();
		// Schedule a Signal command in the queue.
		const UINT64 currentFenceValue = m_FenceValues[m_CurrentFrameIndex];
		commandQueue->Signal(m_Fence, currentFenceValue);

		// Update the frame index.
		m_CurrentFrameIndex = m_Swapchain->GetCurrentBackBufferIndex();

		// If the next frame is not ready to be rendered yet, wait until it is ready.
		if (m_Fence->GetCompletedValue() < m_FenceValues[m_CurrentFrameIndex])
		{
			m_Fence->SetEventOnCompletion(m_FenceValues[m_CurrentFrameIndex], m_FenceEvent);
			WaitForSingleObjectEx(m_FenceEvent, INFINITE, FALSE);
		}

		// Set the fence value for the next frame.
		m_FenceValues[m_CurrentFrameIndex] = currentFenceValue + 1;
	}

}