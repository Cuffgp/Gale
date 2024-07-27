#include "glpch.h"

#include "DirectXIndexBuffer.h"

namespace Gale {

	DirectXIndexBuffer::DirectXIndexBuffer(void* data, uint32_t size) :
		m_Size(size)
	{
		m_IndexCount = size / sizeof(uint32_t);

		m_Buffer = CreateScope<DirectXBuffer>(
			size,
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_RESOURCE_STATE_COPY_DEST);

		SetData(data);
	}

	DirectXIndexBuffer::~DirectXIndexBuffer()
	{

	}

	void DirectXIndexBuffer::SetData(void* data)
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
			D3D12_RESOURCE_STATE_INDEX_BUFFER);

		// Initialize the Index buffer view.
		m_IndexBufferView.BufferLocation = m_Buffer->GetBuffer()->GetGPUVirtualAddress();
		m_IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
		m_IndexBufferView.SizeInBytes = m_Size;
	}

}