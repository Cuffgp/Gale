#include "glpch.h"

#include "DirectXVertexBuffer.h"

namespace Gale {

	DirectXVertexBuffer::DirectXVertexBuffer(void* data, uint32_t size, VertexInput input):
		m_Size(size), m_Input(input)
	{
		CreateBuffer(size);
		SetData(data);
	}

	DirectXVertexBuffer::~DirectXVertexBuffer()
	{

	}

	void DirectXVertexBuffer::CreateBuffer(uint32_t size)
	{
		auto device = DirectXDevice::Get().Device();

		m_Buffer = CreateScope<DirectXBuffer>(size, D3D12_HEAP_TYPE_UPLOAD);

	}

	void DirectXVertexBuffer::SetData(void* data)
	{
		m_Buffer->WriteToBuffer(data);

		// Initialize the vertex buffer view.
		m_VertexBufferView.BufferLocation = m_Buffer->GetBuffer()->GetGPUVirtualAddress();
		m_VertexBufferView.StrideInBytes = m_Input.GetStride();
		m_VertexBufferView.SizeInBytes = m_Size;
	}

}