#pragma once

#include "Gale/Renderer/ShaderData.h"
#include "Gale/Renderer/IndexBuffer.h"

#include "Gale/DirectX/DirectXDevice.h"
#include "Gale/DirectX/DirectXBuffer.h"

namespace Gale {

	class DirectXIndexBuffer : public IndexBuffer
	{
	public:
		DirectXIndexBuffer(void* data, uint32_t size);
		~DirectXIndexBuffer();

		virtual uint32_t GetIndexCount() override { return m_IndexCount; };

		D3D12_INDEX_BUFFER_VIEW GetView() { return m_IndexBufferView; }
		void CreateBuffer(uint32_t size);
		void SetData(void* data);

	private:
		uint32_t m_Size;
		uint32_t m_IndexCount;

		Scope<DirectXBuffer> m_Buffer;
		D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
	};

}
