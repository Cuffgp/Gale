#include "glpch.h"

#include "Gale/Renderer/RendererAPI.h"
#include "Gale/Renderer/DescriptorSet.h"

#include "Gale/DirectX/DirectXDescriptorSet.h"
#include "Gale/Vulkan/VulkanDescriptorSet.h"

namespace Gale {

	Ref<DescriptorSet> DescriptorSet::Create(std::vector<ShaderDescriptor> descriptors)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPI::API::None: { return nullptr; }
		case RendererAPI::API::Vulkan: { return CreateRef<VulkanDescriptorSet>(descriptors); }
		case RendererAPI::API::DirectX: { return CreateRef<DirectXDescriptorSet>(descriptors); }
		}
		GL_ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}


}