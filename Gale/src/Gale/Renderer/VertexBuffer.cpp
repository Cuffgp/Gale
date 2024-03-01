#include "glpch.h"

#include "Gale/Renderer/RendererAPI.h"
#include "Gale/Renderer/VertexBuffer.h"

#include "Gale/Vulkan/VulkanVertexBuffer.h"
#include "Gale/DirectX/DirectXVertexBuffer.h"

namespace Gale {

	Ref<VertexBuffer> VertexBuffer::Create(void* data, uint32_t size, VertexInput input)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPI::API::None: { return nullptr; }
		case RendererAPI::API::Vulkan: { return CreateRef<VulkanVertexBuffer>(data, size, input); }
		case RendererAPI::API::DirectX: { return CreateRef<DirectXVertexBuffer>(data, size, input); }
		}
		GL_ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}

}