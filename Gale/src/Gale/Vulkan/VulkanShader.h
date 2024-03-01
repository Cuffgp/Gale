#pragma once

#include "Gale/Renderer/ShaderData.h"
#include "Gale/Vulkan/VulkanDevice.h"

#include "spirv_reflect/spirv_reflect.h"

namespace Gale {

	class VulkanShader
	{
	public:
		VulkanShader(const std::string& filepath);
		VulkanShader(const std::string& vertPath, const std::string& fragPath);
		~VulkanShader();

		VkShaderModule GetVertModule() { return m_VertModule; }
		VkShaderModule GetFragModule() { return m_FragModule; }

		VertexInput& GetVertexInput() { return m_VertexInput; }
		DescriptorSetMap& GetDescriptorSetMap() { return m_DescriptorMap; }
		PushMap& GetPushMap() { return m_PushMap; }
	private:

		std::string ReadFile( const std::string& filepath);
		std::unordered_map<ShaderStage, std::string> PreProcess(const std::string& source);
		void CompileOrGetSPIRV();

		VkShaderModule CreateShaderModule(const std::vector<uint32_t>& code);

		void Reflect(const std::vector<uint32_t>& code, ShaderStage stage);
		void PrintReflection();
	private:
		std::string m_Filepath;

		std::unordered_map<ShaderStage, std::string> m_Code;
		std::unordered_map<ShaderStage, std::vector<uint32_t>> m_SPIRV;

		VkShaderModule m_VertModule;
		VkShaderModule m_FragModule;

		VertexInput m_VertexInput;
		DescriptorSetMap m_DescriptorMap;
		PushMap m_PushMap;
	};
}