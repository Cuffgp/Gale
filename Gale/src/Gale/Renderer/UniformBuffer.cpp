#include "glpch.h"

#include "Gale/Renderer/RendererAPI.h"
#include "Gale/Renderer/UniformBuffer.h"

#include "Gale/DirectX/DirectXUniformBuffer.h"
#include "Gale/Vulkan/VulkanUniformBuffer.h"

namespace Gale {

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPI::API::None: { return nullptr; }
		case RendererAPI::API::Vulkan: { return CreateRef<VulkanUniformBuffer>(size); }
		case RendererAPI::API::DirectX: { return CreateRef<DirectXUniformBuffer>(size); }
		}
		GL_ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}
}