#include "glpch.h"

#include "Gale/Vulkan/VulkanDescriptorSet.h"

namespace Gale {

	VulkanDescriptorSet::VulkanDescriptorSet(std::vector<ShaderDescriptor> descriptors)
	{
		for (auto& descriptor : descriptors)
		{
			m_DescriptorMap[descriptor.Binding] = descriptor;
		}

		CreateDescriptorSetLayout();
		CreateDescriptorSet();
	}

	VulkanDescriptorSet::~VulkanDescriptorSet()
	{
		auto device = VulkanDevice::Get().Device();
		vkDestroyDescriptorSetLayout(device, m_Layout, nullptr);
	}

	void VulkanDescriptorSet::Bind()
	{

	}

	void VulkanDescriptorSet::CreateDescriptorSetLayout()
	{
		auto device = VulkanDevice::Get().Device();

		std::vector<VkDescriptorSetLayoutBinding> layoutBindings;

		for (auto& descriptor : m_DescriptorMap)
		{
			VkDescriptorSetLayoutBinding uboLayoutBinding{};
			uboLayoutBinding.binding = descriptor.second.Binding;
			uboLayoutBinding.descriptorType = Utils::ShaderAttachmentToVulkan(descriptor.second.Type);
			uboLayoutBinding.descriptorCount = 1;
			uboLayoutBinding.stageFlags = Utils::ShaderStageToVulkan(descriptor.second.ShaderStageBits);
			uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

			layoutBindings.push_back(uboLayoutBinding);
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = layoutBindings.size();
		layoutInfo.pBindings = layoutBindings.data();

		if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &m_Layout) != VK_SUCCESS)
		{
			GL_ERROR("failed to create descriptor set layout!");
		}
	}

	void VulkanDescriptorSet::CreateDescriptorSet()
	{
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = VulkanDevice::Get().DescriptorPool();
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &m_Layout;

		if (vkAllocateDescriptorSets(VulkanDevice::Get().Device(), &allocInfo, &m_DescriptorSet) != VK_SUCCESS)
		{
			GL_ERROR("Failed to allocate descriptor sets!");
		}
	}

}