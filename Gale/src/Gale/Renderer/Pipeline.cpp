#include "glpch.h"

#include "Gale/Renderer/RendererAPI.h"
#include "Gale/Renderer/Pipeline.h"

#include "Gale/Vulkan/VulkanPipeline.h"
#include "Gale/DirectX/DirectXPipeline.h"

namespace Gale {

	Ref<Pipeline> Pipeline::Create(std::string filepath)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPI::API::None: { return nullptr; }
		case RendererAPI::API::Vulkan: { return CreateRef<VulkanPipeline>(filepath); }
		case RendererAPI::API::DirectX: { return CreateRef<DirectXPipeline>(filepath); }
		}
		GL_ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}

	Ref<Pipeline> Pipeline::Create(std::string filepath, DescriptorSetMap descriptorSetMap)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPI::API::None: { return nullptr; }
		case RendererAPI::API::Vulkan: { return CreateRef<VulkanPipeline>(filepath, descriptorSetMap); }
		case RendererAPI::API::DirectX: { return CreateRef<DirectXPipeline>(filepath, descriptorSetMap); }
		}
		GL_ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}

	Ref<Pipeline> Pipeline::Create(std::string vertPath, std::string fragPath)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPI::API::None: { return nullptr; }
		case RendererAPI::API::Vulkan: { return CreateRef<VulkanPipeline>(vertPath, fragPath); }
		case RendererAPI::API::DirectX: { return CreateRef<DirectXPipeline>(vertPath, fragPath); }
		}
		GL_ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}

}