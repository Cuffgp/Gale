#include "glpch.h"

#include "Gale/DirectX/DirectXTexture.h"

namespace Gale {

	DirectXTexture::DirectXTexture(std::string filepath)
	{
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load("filepath", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		size_t imageSize = texWidth * texHeight * 4;

		if (!pixels) 
		{
			GL_ASSERT(false, "Failed to load texture image!");
		}
	}

	DirectXTexture::~DirectXTexture()
	{

	}

}