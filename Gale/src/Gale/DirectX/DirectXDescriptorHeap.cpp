#include "glpch.h"

#include "Gale/DirectX/DirectXDevice.h"
#include "Gale/DirectX/DirectXDescriptorHeap.h"

namespace Gale {

	DirectXDescriptorHeap::DirectXDescriptorHeap()
	{
		GL_INFO("Creating descriptor heap");

		auto device = DirectXDevice::Get().Device();

		D3D12_DESCRIPTOR_HEAP_DESC uniformHeapDesc = {};
		uniformHeapDesc.NumDescriptors = 1000;
		uniformHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		uniformHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		uniformHeapDesc.NodeMask = 0;

		device->CreateDescriptorHeap(&uniformHeapDesc, IID_PPV_ARGS(&m_UniformHeap));

		D3D12_DESCRIPTOR_HEAP_DESC samplerHeapDesc = {};
		samplerHeapDesc.NumDescriptors = 1000;
		samplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
		samplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		samplerHeapDesc.NodeMask = 0;

		device->CreateDescriptorHeap(&samplerHeapDesc, IID_PPV_ARGS(&m_SamplerHeap));

		m_UniformHandle = m_UniformHeap->GetCPUDescriptorHandleForHeapStart();
		m_SamplerHandle = m_UniformHeap->GetCPUDescriptorHandleForHeapStart();
	}

	DirectXDescriptorHeap::~DirectXDescriptorHeap()
	{
		m_UniformHeap->Release();
		m_SamplerHeap->Release();
	}

}