#pragma once

#include "Gale/Renderer/Texture.h"

namespace Gale {

	class DirectXTexture : public Texture
	{
	public:
		DirectXTexture(std::string filepath);
		~DirectXTexture();

	private:

	};

}