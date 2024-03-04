#pragma once

#include "Gale/Renderer/ShaderData.h"

namespace Gale {

	class DescriptorSet
	{
	public:
		virtual ~DescriptorSet() = default;

		static Ref<DescriptorSet> Create(std::vector<ShaderDescriptor> descriptors);

	private:
	};

}