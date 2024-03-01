#pragma once

#include "DirectXDevice.h"

namespace Gale {

	class DirectXBuffer
	{
	public:
		DirectXBuffer(uint32_t size, D3D12_HEAP_TYPE type);
		~DirectXBuffer();

		void Map();
		void Unmap();
		void WriteToBuffer(void* data);
		ID3D12Resource* GetBuffer() { return m_Buffer; }
	private:
		uint32_t m_Size;
		ID3D12Resource* m_Buffer;
		void* m_Mapped = nullptr;
	};


}