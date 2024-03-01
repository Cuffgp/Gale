#include "glpch.h"

#include "Gale/DirectX/DirectXUniformBuffer.h"

namespace Gale {

	DirectXUniformBuffer::DirectXUniformBuffer(uint32_t size):
		m_Size(size)
	{
		auto device = DirectXDevice::Get().Device();

		m_Buffer = CreateScope<DirectXBuffer>(size, D3D12_HEAP_TYPE_UPLOAD);

		D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
		cbvHeapDesc.NumDescriptors = 1;
		cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		device->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_DescriptorHeap));

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_Buffer->GetBuffer()->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = m_Size;
		device->CreateConstantBufferView(&cbvDesc, m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	}

	DirectXUniformBuffer::~DirectXUniformBuffer()
	{

	}

	void DirectXUniformBuffer::SetData(void* data)
	{
		m_Buffer->Map();
		m_Buffer->WriteToBuffer(data);
	}

	void DirectXUniformBuffer::Write(Ref<DescriptorSet> descriptorSet)
	{

	}

}