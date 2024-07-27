#pragma once

#include "Gale/Renderer/DescriptorSet.h"

#include "Gale/DirectX/DirectXDevice.h"

namespace Gale {

	class DirectXDescriptorSet : public DescriptorSet
	{
	public:
		DirectXDescriptorSet(std::vector<ShaderDescriptor> descriptors);
		DirectXDescriptorSet(DescriptorMap descriptorMap);
		~DirectXDescriptorSet();

		virtual DescriptorMap& GetDescriptorMap() override { return m_DescriptorMap; }

	private:
		DescriptorMap m_DescriptorMap;

	};

}