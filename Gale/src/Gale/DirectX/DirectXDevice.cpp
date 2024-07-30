#include "glpch.h"

#include "Gale/DirectX/DirectXDevice.h"

namespace Gale {

	DirectXDevice* DirectXDevice::s_Device = nullptr;

	DirectXDevice::DirectXDevice()
	{
		CreateAdaptor();
		CreateDevice();

		m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator));
		m_CommandAllocator->SetName(L"DeviceCommandAllocator");
		m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));
		m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	}

	DirectXDevice::~DirectXDevice()
	{
		m_CommandAllocator->Release();
		m_CommandQueue->Release();

		if (m_Validation)
		{
			m_DebugController->Release();
			m_DebugDevice->ReportLiveDeviceObjects(D3D12_RLDO_SUMMARY);
			m_DebugDevice->Release();
		}

		m_Device->Release();
		m_Adapter->Release();
		m_Factory->Release();
	}

	void DirectXDevice::Init()
	{
		s_Device = new DirectXDevice();
	}

	void DirectXDevice::Destroy()
	{
		delete s_Device;
		s_Device = nullptr;
	}

	void DirectXDevice::CreateAdaptor()
	{
		GL_INFO("Creating adaptor");
		uint32_t dxgiFactoryFlags = 0;

		// Enable the debug layer (requires the Graphics Tools "optional feature").
		// NOTE: Enabling the debug layer after device creation will invalidate the active device.
		if (m_Validation)
		{
			ID3D12Debug* dc;
			if (D3D12GetDebugInterface(IID_PPV_ARGS(&dc)) != S_OK)
			{
				GL_ERROR("Failed to create Debug interface");
			}

			if (dc->QueryInterface(IID_PPV_ARGS(&m_DebugController)) != S_OK)
			{
				GL_ERROR("Failed to query Debug interface");
			}

			m_DebugController->EnableDebugLayer();
			m_DebugController->SetEnableGPUBasedValidation(true);

			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

			dc->Release();
			dc = nullptr;
		}

		HRESULT result = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_Factory));

		// Select the adapter
		for (UINT adapterIndex = 0;
			DXGI_ERROR_NOT_FOUND != m_Factory->EnumAdapters1(adapterIndex, &m_Adapter);
			++adapterIndex)
		{
			DXGI_ADAPTER_DESC1 desc;
			m_Adapter->GetDesc1(&desc);

			// Don't select the Basic Render Driver adapter.
			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				continue;
			}

			// Check if the adapter supports Direct3D 12, and use that for the rest
			// of the application
			if (SUCCEEDED(D3D12CreateDevice(m_Adapter, D3D_FEATURE_LEVEL_12_0,
				_uuidof(ID3D12Device), nullptr)))
			{
				break;
			}

			// Else we won't use this iteration's adapter, so release it
			m_Adapter->Release();
		}

	}

	void DirectXDevice::CreateDevice()
	{
		// Create the device

		GL_INFO("Creating device");
		if (D3D12CreateDevice(m_Adapter, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&m_Device)) != S_OK)
		{
			GL_ERROR("Failed to create device");
		}

		m_Device->SetName(L"DirectX Device");

		if (m_Validation)
		{
			if ((m_Device->QueryInterface(&m_DebugDevice) != S_OK))
			{
				GL_ERROR("Failed to create debug device");
			}
		}

		// Create the command queue
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		if (m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue)) != S_OK)
		{
			GL_ERROR("Failed to create command queue");
		}

		/*
		if (m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator)) != S_OK)
		{
			GL_ERROR("Failed to create command allocator");
		}
		*/

	}

	ID3D12GraphicsCommandList* DirectXDevice::BeginSingleTimeCommands()
	{
		ID3D12GraphicsCommandList* commandList;
		m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator, nullptr, IID_PPV_ARGS(&commandList));
		commandList->Close();

		m_CommandAllocator->Reset();
		commandList->Reset(m_CommandAllocator, nullptr);
		return commandList;
	}

	void DirectXDevice::EndSingleTimeCommands(ID3D12GraphicsCommandList* commandList)
	{
		commandList->Close();
		ID3D12CommandList* ppCommandLists[] = { commandList };
		m_CommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
		commandList->Release();

		m_CommandQueue->Signal(m_Fence, ++m_FenceValue);
	
		m_Fence->SetEventOnCompletion(m_FenceValue, m_FenceEvent);
		WaitForSingleObjectEx(m_FenceEvent, INFINITE, FALSE);
	}

	void DirectXDevice::CopyBuffer(ID3D12Resource* srcBuffer, ID3D12Resource* dstBuffer, uint32_t size)
	{
		auto commandList = BeginSingleTimeCommands();

		commandList->CopyBufferRegion(
			dstBuffer,
			0,
			srcBuffer,
			0,
			size);

		EndSingleTimeCommands(commandList);
	}

	void DirectXDevice::CopyAndTransitionBuffer(
		ID3D12Resource* srcBuffer,
		ID3D12Resource* dstBuffer,
		uint32_t size,
		D3D12_RESOURCE_STATES state)
	{
		auto commandList = BeginSingleTimeCommands();

		// Copy
		commandList->CopyBufferRegion(
			dstBuffer,
			0,
			srcBuffer,
			0,
			size);

		// Transition
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			dstBuffer,
			D3D12_RESOURCE_STATE_COPY_DEST,
			state);

		commandList->ResourceBarrier(1, &barrier);

		EndSingleTimeCommands(commandList);
	}


	void DirectXDevice::CopyBufferToImage(ID3D12Resource* buffer, ID3D12Resource* image, uint32_t size)
	{

	}
}