#pragma once

#include "Gale/Renderer/ShaderData.h"

namespace Gale {

	class DescriptorSet
	{
	public:
		virtual ~DescriptorSet() = default;

		virtual DescriptorMap& GetDescriptorMap() = 0;

		static Ref<DescriptorSet> Create(std::vector<ShaderDescriptor> descriptors);
		static Ref<DescriptorSet> Create(DescriptorMap descriptorMap);

	private:
	};

}