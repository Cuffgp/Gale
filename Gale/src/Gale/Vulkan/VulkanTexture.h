#pragma once

#include "Gale/Renderer/Texture.h"

namespace Gale {

	class VulkanTexture : public Texture
	{
	public:
		VulkanTexture(std::string filepath);
		~VulkanTexture();

	private:

	};

}