#pragma once

#include "Gale/Renderer/Texture.h"

#include "Gale/Vulkan/Vulkan.h"

namespace Gale {

	class VulkanTexture : public Texture
	{
	public:
		VulkanTexture(std::string filepath);
		~VulkanTexture();

	private:
		VkImage m_Image;
		VkDeviceMemory m_ImageMemory;
		VkImageView m_ImageView;
		VkSampler m_Sampler;

		uint32_t m_Width;
		uint32_t m_Height;
	};

}