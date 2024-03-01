#include "glpch.h"

#include "DirectXIndexBuffer.h"

namespace Gale {

	DirectXIndexBuffer::DirectXIndexBuffer(void* data, uint32_t size) :
		m_Size(size)
	{
		m_IndexCount = size / sizeof(uint32_t);

		CreateBuffer(size);
		SetData(data);
	}

	DirectXIndexBuffer::~DirectXIndexBuffer()
	{

	}

	void DirectXIndexBuffer::CreateBuffer(uint32_t size)
	{
		m_Buffer = CreateScope<DirectXBuffer>(size, D3D12_HEAP_TYPE_UPLOAD);
	}

	void DirectXIndexBuffer::SetData(void* data)
	{
		m_Buffer->WriteToBuffer(data);

		// Initialize the Index buffer view.
		m_IndexBufferView.BufferLocation = m_Buffer->GetBuffer()->GetGPUVirtualAddress();
		m_IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
		m_IndexBufferView.SizeInBytes = m_Size;
	}

}