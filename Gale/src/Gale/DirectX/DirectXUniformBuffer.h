#pragma once

#include "Gale/Renderer/UniformBuffer.h"

#include "Gale/DirectX/DirectXBuffer.h"

namespace Gale {

	class DirectXUniformBuffer : public UniformBuffer
	{
	public:
		DirectXUniformBuffer(uint32_t size);
		~DirectXUniformBuffer();

		virtual void SetData(void* data) override;

		virtual void Write(Ref<DescriptorSet> descriptorSet) override;

		D3D12_GPU_VIRTUAL_ADDRESS GetGPUAddress() { return m_Buffer->GetBuffer()->GetGPUVirtualAddress(); }
	private:
		Scope<DirectXBuffer> m_Buffer;
		uint32_t m_Size;
		ID3D12DescriptorHeap* m_DescriptorHeap;
	};

}