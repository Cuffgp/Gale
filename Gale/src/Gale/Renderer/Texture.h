#pragma once

#include "Gale/Core/Base.h"

namespace Gale {

	class Texture
	{
	public:

		virtual ~Texture() = default;

		static Ref<Texture> Create(const std::string filepath);

	private:

	};

}