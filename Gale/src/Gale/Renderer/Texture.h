#pragma once

#include "Gale/Core/Base.h"
#include "Gale/Renderer/DescriptorSet.h"

namespace Gale {

	class Texture
	{
	public:

		virtual ~Texture() = default;

		static Ref<Texture> Create(const std::string filepath);

		virtual void Write(Ref<DescriptorSet> descriptorSet) = 0;
	private:

	};

}