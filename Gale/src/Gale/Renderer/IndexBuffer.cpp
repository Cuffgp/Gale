#include "glpch.h"

#include "Gale/Renderer/IndexBuffer.h"
#include "Gale/Renderer/RendererAPI.h"

#include "Gale/Vulkan/VulkanIndexBuffer.h"
#include "Gale/DirectX/DirectXIndexBuffer.h"

namespace Gale {

	Ref<IndexBuffer> IndexBuffer::Create(void* data, uint32_t size)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPI::API::None: { return nullptr; }
		case RendererAPI::API::Vulkan: { return CreateRef<VulkanIndexBuffer>(data, size); }
		case RendererAPI::API::DirectX: { return CreateRef<DirectXIndexBuffer>(data, size); }
		}
		GL_ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}

}