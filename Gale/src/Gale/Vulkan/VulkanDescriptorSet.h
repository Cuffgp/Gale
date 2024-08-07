#pragma once

#include "Gale/Renderer/DescriptorSet.h"

#include "Gale/Vulkan/VulkanDevice.h"

namespace Gale {

	class VulkanDescriptorSet : public DescriptorSet
	{
	public:
		VulkanDescriptorSet(std::vector<ShaderDescriptor> descriptors);
		VulkanDescriptorSet(DescriptorMap descriptorMap);
		~VulkanDescriptorSet();

		virtual DescriptorMap& GetDescriptorMap() override { return m_DescriptorMap; }

		VkDescriptorSet GetDescriptorSet() { return m_DescriptorSet; }
	private:

		void CreateDescriptorSetLayout();
		void CreateDescriptorSet();

		DescriptorMap m_DescriptorMap;
		VkDescriptorSetLayout m_Layout;
		VkDescriptorSet m_DescriptorSet;

	};

}