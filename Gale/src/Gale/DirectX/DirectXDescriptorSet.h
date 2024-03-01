#pragma once

#include "Gale/Renderer/DescriptorSet.h"

namespace Gale {

	class DirectXDescriptorSet : public DescriptorSet
	{
	public:
		DirectXDescriptorSet(std::vector<ShaderDescriptor> descriptors);
		~DirectXDescriptorSet();

		virtual void Bind() override;
	private:

	};

}