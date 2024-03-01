#include "glpch.h"

#include "Gale/Vulkan/VulkanShader.h"

#include <shaderc/shaderc.hpp>
#include <spirv_reflect/spirv_reflect.c>

namespace Gale {

	namespace Utils {

		static ShaderStage ShaderTypeFromString(const std::string& type)
		{
			if (type == "vertex")
				return ShaderStage::Vertex;
			if (type == "fragment" || type == "pixel")
				return ShaderStage::Fragment;

			GL_ASSERT(false, "Unknown shader type!");
			return (ShaderStage)0;
		}

		static shaderc_shader_kind ShaderStageToShaderc(ShaderStage stage)
		{
			if (stage & ShaderStage::Vertex) { return shaderc_vertex_shader; }
			if (stage & ShaderStage::Fragment) { return shaderc_fragment_shader; }

			GL_ASSERT(false, "Unknown shader stage");
			return (shaderc_shader_kind)0;
		}

		static const char* GetCacheDirectory()
		{
			// TODO: make sure the assets directory is valid
			return "assets/cache/shader/vulkan";
		}

		static void CreateCacheDirectoryIfNeeded()
		{
			std::string cacheDirectory = GetCacheDirectory();
			if (!std::filesystem::exists(cacheDirectory))
				std::filesystem::create_directories(cacheDirectory);
		}

		static const char* ShaderStageCachedFileExtension(ShaderStage stage)
		{
			switch (stage)
			{
			case ShaderStage::Vertex:    return ".cached_vulkan.vert.spv";
			case ShaderStage::Fragment:  return ".cached_vulkan.frag.spv";
			}
			GL_ASSERT(false, "Unknown");
			return "";
		}

		static ShaderDataType SpvReflectTypeToShaderType(SpvReflectFormat format)
		{
			switch (format)
			{
			case SPV_REFLECT_FORMAT_R32_SFLOAT: return ShaderDataType::Float;
			case SPV_REFLECT_FORMAT_R32G32_SFLOAT: return ShaderDataType::Float2;
			case SPV_REFLECT_FORMAT_R32G32B32_SFLOAT: return ShaderDataType::Float3;
			case SPV_REFLECT_FORMAT_R32G32B32A32_SFLOAT: return ShaderDataType::Float4;
			}
			GL_ASSERT(false, "Unknown shader data type");
			return ShaderDataType::None;
		}

		static ShaderDescriptorType SpvReflectDescriptorTypeToShader(SpvReflectDescriptorType type)
		{
			switch (type)
			{
			case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER: return ShaderDescriptorType::Uniform;
			case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: return ShaderDescriptorType::ImageSampler;
			}
			GL_ASSERT(false, "Unknown shader descriptor type");
			return ShaderDescriptorType::None;
		}

	}

	VulkanShader::VulkanShader(const std::string& filepath)
	{
		m_Filepath = filepath;

		auto shaderCode = ReadFile(filepath+".glsl");
		m_Code = PreProcess(shaderCode);
		CompileOrGetSPIRV();

		m_VertModule = CreateShaderModule(m_SPIRV[ShaderStage::Vertex]);
		m_FragModule = CreateShaderModule(m_SPIRV[ShaderStage::Fragment]);

		Reflect(m_SPIRV[ShaderStage::Vertex], ShaderStage::Vertex);
		Reflect(m_SPIRV[ShaderStage::Fragment], ShaderStage::Fragment);

		PrintReflection();
	}

	VulkanShader::VulkanShader(const std::string& vertPath, const std::string& fragPath)
	{
		m_Filepath = vertPath;

		auto vertCode = ReadFile(vertPath);
		auto fragCode = ReadFile(fragPath);

		m_Code[ShaderStage::Vertex] = vertCode;
		m_Code[ShaderStage::Fragment] = fragCode;
		CompileOrGetSPIRV();

		m_VertModule = CreateShaderModule(m_SPIRV[ShaderStage::Vertex]);
		m_FragModule = CreateShaderModule(m_SPIRV[ShaderStage::Fragment]);

		Reflect(m_SPIRV[ShaderStage::Vertex], ShaderStage::Vertex);
		Reflect(m_SPIRV[ShaderStage::Fragment], ShaderStage::Fragment);

		PrintReflection();
	}

	VulkanShader::~VulkanShader()
	{
		auto device = VulkanDevice::Get().Device();
		vkDestroyShaderModule(device, m_VertModule, nullptr);
		vkDestroyShaderModule(device, m_FragModule, nullptr);
	}

	std::string VulkanShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				GL_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			GL_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<ShaderStage, std::string> VulkanShader::PreProcess(const std::string& source)
	{
		std::unordered_map<ShaderStage, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			GL_ASSERT((eol != std::string::npos), "Syntax error");
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			GL_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			GL_ASSERT((nextLinePos != std::string::npos), "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void VulkanShader::CompileOrGetSPIRV()
	{

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		const bool optimize = false;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		Utils::CreateCacheDirectoryIfNeeded();

		auto& shaderData = m_SPIRV;
		shaderData.clear();

		for (auto&& [stage, source] : m_Code)
		{
			std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();
			std::filesystem::path shaderFilePath = m_Filepath;

			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::ShaderStageCachedFileExtension(stage));

			// If the shader exists in the cache read it
			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}

			// Otherwise compile and save it to the cache
			else
			{
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::ShaderStageToShaderc(stage), Utils::ShaderStageToString(stage).c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					GL_ERROR("{}", module.GetErrorMessage());
					GL_ASSERT(false, "Shader compile fail");
				}

				m_SPIRV[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = m_SPIRV[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}

	VkShaderModule VulkanShader::CreateShaderModule(const std::vector<uint32_t>& code)
	{
		auto device = VulkanDevice::Get().Device();

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size() * sizeof(uint32_t);
		createInfo.pCode = code.data();

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		{
			GL_ERROR("Failed to create shader module!");
		}

		return shaderModule;
	}

	void VulkanShader::Reflect(const std::vector<uint32_t>& code, ShaderStage stage)
	{
		SpvReflectShaderModule module;
		SpvReflectResult result = spvReflectCreateShaderModule(code.size() * sizeof(uint32_t), code.data(), &module);
		GL_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS, "Failed to reflect shader module");

		// Reflect the input of the vertex shader
		if (stage & ShaderStage::Vertex)
		{
			uint32_t inputCount = 0;
			result = spvReflectEnumerateInputVariables(&module, &inputCount, NULL);
			assert(result == SPV_REFLECT_RESULT_SUCCESS);

			std::vector<SpvReflectInterfaceVariable*> inputVars(inputCount);
			result = spvReflectEnumerateInputVariables(&module, &inputCount, inputVars.data());
			assert(result == SPV_REFLECT_RESULT_SUCCESS);

			for (auto input : inputVars)
			{
				std::string name;
				input->name == 0 ? name = "name" : name = input->name;
				ShaderDataType format = Utils::SpvReflectTypeToShaderType(input->format);
				uint32_t location = input->location;

				m_VertexInput.PushAttribute(VertexInputAttribute(name, format, location));
			}

			// Sort the vertex inputs by location
			std::sort(m_VertexInput.begin(), m_VertexInput.end(),
				[](const VertexInputAttribute& lhs, const VertexInputAttribute& rhs) {
					return lhs.Location < rhs.Location; 
				});
			m_VertexInput.CalculateOffsetAndStride();
		}

		// Reflect the descriptor sets
		uint32_t descriptorSetCount = 0;
		result = spvReflectEnumerateDescriptorSets(&module, &descriptorSetCount, NULL);
		assert(result == SPV_REFLECT_RESULT_SUCCESS);

		std::vector<SpvReflectDescriptorSet*> sets(descriptorSetCount);
		result = spvReflectEnumerateDescriptorSets(&module, &descriptorSetCount, sets.data());
		assert(result == SPV_REFLECT_RESULT_SUCCESS);

		for (int set = 0; set < descriptorSetCount; set++)
		{
			const SpvReflectDescriptorSet& reflectionSet = *(sets[set]);

			for (int i = 0; i < reflectionSet.binding_count; i++)
			{
				const SpvReflectDescriptorBinding& reflectionBinding = *(reflectionSet.bindings[i]);

				uint32_t binding = reflectionBinding.binding;
				SpvReflectDescriptorType bindingType = reflectionBinding.descriptor_type;
				ShaderDescriptorType shaderType = Utils::SpvReflectDescriptorTypeToShader(bindingType);

				if (m_DescriptorMap.count(set) && m_DescriptorMap[set].count(binding))
				{
					m_DescriptorMap[set][binding].ShaderStageBits |= stage;
				}

				else
				{
					m_DescriptorMap[set][binding] = ShaderDescriptor(set, binding, stage, shaderType);
				}
			}
		}

		// Reflect the push constant range
		uint32_t pushCount = 0;
		result = spvReflectEnumeratePushConstantBlocks(&module, &pushCount, NULL);
		assert(result == SPV_REFLECT_RESULT_SUCCESS);

		std::vector<SpvReflectBlockVariable*> pushBlock(pushCount);
		result = spvReflectEnumeratePushConstantBlocks(&module, &pushCount, pushBlock.data());
		assert(result == SPV_REFLECT_RESULT_SUCCESS);

		for (auto push : pushBlock)
		{
			uint32_t offset = push->offset;
			uint32_t size = push->size;

			// If a push map already exists add this stage to it 
			if (m_PushMap.count(offset))
			{
				m_PushMap[offset].ShaderStageBits |= stage;
			}
			// Otherwise make a new push map for this offset
			else
			{
				m_PushMap[offset] = PushBlock(stage, offset, size);
			}

		}

		spvReflectDestroyShaderModule(&module);
	}

	void VulkanShader::PrintReflection()
	{
		GL_TRACE("-------------------------------------------");
		GL_TRACE("------------ Shader Reflection ------------");
		GL_TRACE("------------ Vertex Input -----------------");
		m_VertexInput.Log();

		GL_TRACE("------------ Descriptor Sets --------------");
		for (const auto& setMap : m_DescriptorMap)
		{
			uint32_t set = setMap.first;
			GL_TRACE("Descriptor Set {}", set);
			for (const auto& binding : setMap.second)
			{
				ShaderDescriptor descriptor = binding.second;
				GL_TRACE("Binding {}, Stage {}, Type {}",
					binding.first, descriptor.ShaderStageBits, Utils::ShaderDescriptorTypeToString(descriptor.Type));
			}
		}

		GL_TRACE("------------ Push Constants --------------");
		for (const auto& push : m_PushMap)
		{
			GL_TRACE("Push Constant Stage {}, Offset {}, Size {}",
				push.second.ShaderStageBits, push.first, push.second.Size);
		}
		GL_TRACE("-------------------------------------------");

	}

}