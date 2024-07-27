#pragma once

#include "Gale/Renderer/ShaderData.h"
#include "Gale/Renderer/VertexBuffer.h"

#include "Gale/DirectX/DirectXDevice.h"
#include "Gale/DirectX/DirectXBuffer.h"

namespace Gale {

	class DirectXVertexBuffer : public VertexBuffer
	{
	public:
		DirectXVertexBuffer(void* data, uint32_t size, VertexInput input);
		~DirectXVertexBuffer();

		virtual const VertexInput& GetInput() const override { return m_Input; }
		virtual void SetData(void* data) override;

		D3D12_VERTEX_BUFFER_VIEW GetView() { return m_VertexBufferView; }
	private:

		uint32_t m_Size;
		VertexInput m_Input;

		Scope<DirectXBuffer> m_Buffer;
		D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
	};

}
