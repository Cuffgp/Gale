#pragma once

#include "Gale/Renderer/DescriptorSet.h"

#include "Gale/DirectX/DirectXDevice.h"

namespace Gale {

	class DirectXDescriptorSet : public DescriptorSet
	{
	public:
		DirectXDescriptorSet(std::vector<ShaderDescriptor> descriptors);
		~DirectXDescriptorSet();

		ID3D12DescriptorHeap* GetUniformHeap() { return m_UniformHeap; }
	private:
		DescriptorMap m_DescriptorMap;

		ID3D12DescriptorHeap* m_UniformHeap;
	};

}