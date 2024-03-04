#include "glpch.h"

#include "Gale/DirectX/DirectXDescriptorSet.h"

namespace Gale {

	DirectXDescriptorSet::DirectXDescriptorSet(std::vector<ShaderDescriptor> descriptors)
	{
		auto device = DirectXDevice::Get().Device();

		for (auto& descriptor : descriptors)
		{
			m_DescriptorMap[descriptor.Binding] = descriptor;
		}
		//D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT

		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.NumDescriptors = 1;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_UniformHeap));
	}

	DirectXDescriptorSet::~DirectXDescriptorSet()
	{

	}

}