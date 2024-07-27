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

	}

	DirectXDescriptorSet::DirectXDescriptorSet(DescriptorMap descriptorMap)
	{
		auto device = DirectXDevice::Get().Device();

		m_DescriptorMap = descriptorMap;

	}

	DirectXDescriptorSet::~DirectXDescriptorSet()
	{

	}

}