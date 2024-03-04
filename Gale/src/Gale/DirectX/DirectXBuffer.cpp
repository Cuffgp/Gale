#include "glpch.h"

#include "Gale/DirectX/DirectXBuffer.h"

namespace Gale {

	DirectXBuffer::DirectXBuffer(uint32_t size, D3D12_HEAP_TYPE type, uint32_t alignedSize):
		m_Size(size), m_AlignedSize(alignedSize)
	{
		if (alignedSize == 0)
		{
			m_AlignedSize = size;
		}


		auto device = DirectXDevice::Get().Device();

		D3D12_HEAP_PROPERTIES heapProps;
		heapProps.Type = type;
		heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProps.CreationNodeMask = 1;
		heapProps.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC resourceDesc;
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Alignment = 0;
		resourceDesc.Width = m_AlignedSize;
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.SampleDesc.Quality = 0;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		auto res = device->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_Buffer));

		m_Buffer->SetName(L"DirectXBuffer");

		if (res != S_OK)
		{
			GL_ERROR("Failed to create buffer");
		}
	}

	DirectXBuffer::~DirectXBuffer()
	{
		if (m_Mapped)
		{
			Unmap();
		}
		m_Buffer->Release();
	}

	void DirectXBuffer::Map()
	{
		m_Buffer->Map(0, nullptr, &m_Mapped);
	}

	void DirectXBuffer::Unmap()
	{
		m_Buffer->Unmap(0, nullptr);
		m_Mapped = nullptr;
	}

	void DirectXBuffer::WriteToBuffer(void* data, uint32_t size)
	{
		if (!m_Mapped)
		{
			Map();
		}

		GL_ASSERT(m_Mapped, "Memory must be mapped to write");
		if (size == 0)
			memcpy(m_Mapped, data, m_Size);
		else
			memcpy(m_Mapped, data, size);
	}

}