#pragma once

#include "Gale/Renderer/ShaderData.h"
#include "Gale/Renderer/Pipeline.h"

#include "Gale/Vulkan/VulkanShader.h"

#include "Vulkan.h"

namespace Gale {

	class VulkanPipeline : public Pipeline
	{
	public:
		VulkanPipeline(std::string filepath);
		VulkanPipeline(std::string vertPath, std::string fragPath);
		~VulkanPipeline();

		VkPipeline GetPipeline() { return m_Pipeline; }
		VkPipelineLayout GetPipelineLayout() { return m_PipelineLayout; }

	private:
		void CreateDescriptorSetLayout(DescriptorSetMap descriptorSetMap);
		void CreatePipelineLayout();
		void CreatePipeline();

	private:
		VkPipeline m_Pipeline;
		VkPipelineLayout m_PipelineLayout;

		Scope<VulkanShader> m_Shader;
		VertexInput m_Input;
		DescriptorSetMap m_DescriptorSetMap;

		std::vector<VkDescriptorSetLayout> m_DescriptorSetLayouts;
	};

}