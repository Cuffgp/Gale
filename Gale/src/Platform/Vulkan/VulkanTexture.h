#pragma once

#include "VulkanDevice.h"

namespace Gale {

	class VulkanTexture
	{
	public:

		VulkanTexture(Ref<VulkanDevice> device, const std::string& texFilepath);
		~VulkanTexture();

		VkDescriptorSetLayoutBinding getDescriptor() { return samplerLayoutBinding; }
		VkImageView getView() { return textureImageView; }
		VkSampler getSampler() { return textureSampler; }

	private:

		void createTexture(const std::string& texFilepath);
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void createTextureImageView();
		void createTextureSampler();
		void setDescriptor();

		Ref<VulkanDevice> m_Device;

		VkBuffer textureBuffer;
		VkDeviceMemory textureBufferMemory;
		VkImage textureImage;
		VkDeviceMemory textureImageMemory;
		VkImageView textureImageView;
		VkSampler textureSampler;

		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	};
}