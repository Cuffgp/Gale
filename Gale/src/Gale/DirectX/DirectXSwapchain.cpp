#include "glpch.h"

#include "Gale/Core/Application.h"

#include "Gale/DirectX/DirectXSwapchain.h"

namespace Gale {

	DirectXSwapchain::DirectXSwapchain()
	{
		CreateSwapchain();
		CreateFramebuffer();
		CreateDepthResources();
	}

	DirectXSwapchain::~DirectXSwapchain()
	{
		// Free Framebuffers
		for (size_t i = 0; i < m_BackbufferCount; ++i)
		{
			if (m_RenderTargets[i])
			{
				m_RenderTargets[i]->Release();
				m_RenderTargets[i] = nullptr;
			}
		}

		if (m_RenderTargetViewHeap)
		{
			m_RenderTargetViewHeap->Release();
			m_RenderTargetViewHeap = nullptr;
		}

		if (m_DepthBuffer)
		{
			m_DepthBuffer->Release();
			m_DepthBuffer = nullptr;
		}

		if (m_DepthBuffersViewHeap)
		{
			m_DepthBuffersViewHeap->Release();
			m_DepthBuffersViewHeap = nullptr;
		}

		m_Swapchain->Release();
		m_Swapchain = nullptr;
	}

	void DirectXSwapchain::CreateSwapchain()
	{
		uint32_t width = Application::Get().GetWindow().Width();
		uint32_t height = Application::Get().GetWindow().Height();

		m_Width = width;
		m_Height = height;

		DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
		swapchainDesc.BufferCount = m_BackbufferCount;
		swapchainDesc.Width = width;
		swapchainDesc.Height = height;
		swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapchainDesc.SampleDesc.Count = 1;

		IDXGISwapChain1* swapchain = nullptr;

		auto result = DirectXDevice::Get().Factory()->CreateSwapChainForHwnd(
			DirectXDevice::Get().CommandQueue(),
			Application::Get().GetWindow().GetRawWindowPointer(),
			&swapchainDesc,
			nullptr,
			nullptr,
			&swapchain
		);

		DirectXDevice::Get().Factory()->MakeWindowAssociation(
			Application::Get().GetWindow().GetRawWindowPointer(),
			DXGI_MWA_NO_ALT_ENTER
		);

		if (result != S_OK)
		{
			GL_ERROR("Failed to create swapchain");
		}

		auto swapchainSupport = swapchain->QueryInterface(__uuidof(IDXGISwapChain3), (void**)&swapchain);
		if (swapchainSupport == S_OK)
		{
			m_Swapchain = (IDXGISwapChain3*)swapchain;
		}

		m_CurrentFrameIndex = m_Swapchain->GetCurrentBackBufferIndex();
	}

	void DirectXSwapchain::CreateFramebuffer()
	{
		auto device = DirectXDevice::Get().Device();

		// Describe and create a render target view (RTV) descriptor heap.
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = m_BackbufferCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		auto result = device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RenderTargetViewHeap));
		if (result != S_OK)
		{
			GL_ERROR("Failed to create descriptor heap");
		}

		m_RtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RenderTargetViewHeap->GetCPUDescriptorHandleForHeapStart());

		// Create a RTV for each frame.
		for (UINT n = 0; n < m_BackbufferCount; n++)
		{
			m_Swapchain->GetBuffer(n, IID_PPV_ARGS(&m_RenderTargets[n]));
			device->CreateRenderTargetView(m_RenderTargets[n], nullptr, rtvHandle);
			rtvHandle.ptr += (1 * m_RtvDescriptorSize);
		}
	}

	void DirectXSwapchain::CreateDepthResources()
	{
		auto device = DirectXDevice::Get().Device();

		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		dsvHeapDesc.NodeMask = NULL;

		device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_DepthBuffersViewHeap));

		D3D12_DEPTH_STENCIL_VIEW_DESC dsViewDesk{};
		dsViewDesk.Format = DXGI_FORMAT_D32_FLOAT;
		dsViewDesk.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsViewDesk.Flags = D3D12_DSV_FLAG_NONE;
		dsViewDesk.Texture2D.MipSlice = 0;

		// Describe clear value 
		D3D12_CLEAR_VALUE clearValueDs{};
		clearValueDs.Format = DXGI_FORMAT_D32_FLOAT;
		clearValueDs.DepthStencil.Depth = 1.0f;
		clearValueDs.DepthStencil.Stencil = 0;

		device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, m_Width, m_Height,
				1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValueDs,
			IID_PPV_ARGS(&m_DepthBuffer)
		);
		m_DepthBuffer->SetName(L"Depth/Stencil Resource Heap");

		D3D12_CPU_DESCRIPTOR_HANDLE heapHandleDsv = m_DepthBuffersViewHeap->GetCPUDescriptorHandleForHeapStart();

		device->CreateDepthStencilView(m_DepthBuffer, &dsViewDesk, heapHandleDsv);


	}

}