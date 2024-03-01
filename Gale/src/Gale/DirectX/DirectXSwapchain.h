#pragma once

#include "Gale/DirectX/DirectXDevice.h"

namespace Gale {

	class DirectXSwapchain
	{
	public:
		DirectXSwapchain();
		~DirectXSwapchain();

		void CreateSwapchain();
		void CreateFramebuffer();
		void CreateDepthResources();

		void Present(uint32_t synchInterval, uint32_t flags) { m_Swapchain->Present(synchInterval, flags); }

		ID3D12Resource* GetRenderTarget(uint32_t imageIndex) { return m_RenderTargets[imageIndex]; }
		ID3D12DescriptorHeap* GetRTVHeap() { return m_RenderTargetViewHeap; }
		ID3D12DescriptorHeap* GetDSVHeap() { return m_DepthBuffersViewHeap; }
		uint32_t GetRTVDescriptorSize() { return m_RtvDescriptorSize; }

		uint32_t GetWidth() { return m_Width; }
		uint32_t GetHeight() { return m_Height; }
		uint32_t GetCurrentBackBufferIndex() { return m_Swapchain->GetCurrentBackBufferIndex(); }
	private:

		static const uint32_t m_BackbufferCount = 2;
		uint32_t m_CurrentFrameIndex;
		uint32_t m_Width;
		uint32_t m_Height;

		IDXGISwapChain3* m_Swapchain;

		ID3D12DescriptorHeap* m_RenderTargetViewHeap;
		ID3D12Resource* m_RenderTargets[m_BackbufferCount];
		uint32_t m_RtvDescriptorSize;

		ID3D12DescriptorHeap* m_DepthBuffersViewHeap;
		ID3D12Resource* m_DepthBuffer;


	};

}