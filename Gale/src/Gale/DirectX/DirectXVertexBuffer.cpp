#include "glpch.h"

#include "DirectXVertexBuffer.h"

namespace Gale {

	DirectXVertexBuffer::DirectXVertexBuffer(void* data, uint32_t size, VertexInput input):
		m_Size(size), m_Input(input)
	{
		auto device = DirectXDevice::Get().Device();

		m_Buffer = CreateScope<DirectXBuffer>(
			size,
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_RESOURCE_STATE_COPY_DEST);

		SetData(data);
	}

	DirectXVertexBuffer::~DirectXVertexBuffer()
	{

	}

	void DirectXVertexBuffer::SetData(void* data)
	{
		DirectXBuffer uploadBuffer(
			m_Size,
			D3D12_HEAP_TYPE_UPLOAD,
			D3D12_RESOURCE_STATE_GENERIC_READ);

		uploadBuffer.WriteToBuffer(data, m_Size);

		//m_Buffer->WriteToBuffer(data);
		DirectXDevice::Get().CopyAndTransitionBuffer(
			uploadBuffer.GetBuffer(),
			m_Buffer->GetBuffer(),
			m_Size,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

		// Initialize the vertex buffer view.
		m_VertexBufferView.BufferLocation = m_Buffer->GetBuffer()->GetGPUVirtualAddress();
		m_VertexBufferView.StrideInBytes = m_Input.GetStride();
		m_VertexBufferView.SizeInBytes = m_Size;
	}

}