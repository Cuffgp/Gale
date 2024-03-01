#pragma once

#include "Gale/Renderer/Pipeline.h"
#include "Gale/Renderer/UniformBuffer.h"

#include "Gale/Vulkan/VulkanBuffer.h"

namespace Gale {

	class VulkanUniformBuffer : public UniformBuffer
	{
	public:
		VulkanUniformBuffer(uint32_t size);
		~VulkanUniformBuffer();

		virtual void SetData(void* data) override;

		virtual void Write(Ref<DescriptorSet> descriptorSet) override;

		void WriteToSet(VkDescriptorSet set, uint32_t binding);
	private:
		uint32_t m_Size;
		Scope<VulkanBuffer> m_Buffer;
		VkDescriptorBufferInfo m_DescriptorInfo;
	};

}