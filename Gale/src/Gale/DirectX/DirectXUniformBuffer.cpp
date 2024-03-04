#include "glpch.h"

#include "Gale/Core/Utilities.h"

#include "Gale/DirectX/DirectXDescriptorSet.h"
#include "Gale/DirectX/DirectXUniformBuffer.h"

namespace Gale {

	DirectXUniformBuffer::DirectXUniformBuffer(uint32_t size):
		m_Size(size)
	{
		auto device = DirectXDevice::Get().Device();

		m_AlignedSize = Utils::AlignedSize(size, 256);

		m_Buffer = CreateScope<DirectXBuffer>(m_AlignedSize, D3D12_HEAP_TYPE_UPLOAD);


	}

	DirectXUniformBuffer::~DirectXUniformBuffer()
	{

	}

	void DirectXUniformBuffer::SetData(void* data)
	{
		m_Buffer->Map();
		m_Buffer->WriteToBuffer(data, m_AlignedSize);
	}

	void DirectXUniformBuffer::Write(Ref<DescriptorSet> descriptorSet)
	{
		auto device = DirectXDevice::Get().Device();
		auto heap = std::static_pointer_cast<DirectXDescriptorSet>(descriptorSet)->GetUniformHeap();

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_Buffer->GetBuffer()->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = m_AlignedSize;
		device->CreateConstantBufferView(&cbvDesc, heap->GetCPUDescriptorHandleForHeapStart());

	}

}