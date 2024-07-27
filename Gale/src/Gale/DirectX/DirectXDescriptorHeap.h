#pragma once

#include "Gale/DirectX/DirectX.h"

namespace Gale {

	class DirectXDescriptorHeap
	{
	public:

		DirectXDescriptorHeap();
		~DirectXDescriptorHeap();

		ID3D12DescriptorHeap* UniformHeap() { return m_UniformHeap; }
		ID3D12DescriptorHeap* SamplerHeap() { return m_SamplerHeap; }

	private:
		ID3D12DescriptorHeap* m_UniformHeap;
		ID3D12DescriptorHeap* m_SamplerHeap;

		D3D12_CPU_DESCRIPTOR_HANDLE m_UniformHandle;
		D3D12_CPU_DESCRIPTOR_HANDLE m_SamplerHandle;
	};


}