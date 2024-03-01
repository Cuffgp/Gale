#pragma once

#include "DirectX.h"

namespace Gale {

	class DirectXDevice
	{
	public:
		DirectXDevice(const DirectXDevice&) = delete;
		DirectXDevice& operator=(const DirectXDevice&) = delete;

		static void DirectXDevice::Init();
		static void DirectXDevice::Destroy();

		ID3D12Device* Device() { return m_Device; }
		IDXGIFactory4* Factory() { return m_Factory; }
		ID3D12CommandQueue* CommandQueue() { return m_CommandQueue; }
		//ID3D12CommandAllocator* CommandAllocator() { return m_CommandAllocator; }

		static inline DirectXDevice& Get() { return *s_Device; }
		static const bool m_Validation = true;

		// Helper functions
		ID3D12GraphicsCommandList* BeginSingleTimeCommands();
		void EndSingleTimeCommands(ID3D12GraphicsCommandList* commandList);
	private:
		DirectXDevice();
		~DirectXDevice();

		void CreateAdaptor();
		void CreateDevice();
	private:

		ID3D12Debug1* m_DebugController = nullptr;
		ID3D12DebugDevice* m_DebugDevice = nullptr;

		IDXGIFactory4* m_Factory;
		IDXGIAdapter1* m_Adapter;
		ID3D12Device* m_Device;

		ID3D12CommandQueue* m_CommandQueue;

		// For single time commands
		ID3D12CommandAllocator* m_CommandAllocator;
		ID3D12Fence* m_Fence;
		uint64_t m_FenceValue = 0;
		HANDLE m_FenceEvent;

		static DirectXDevice* s_Device;
	};

}