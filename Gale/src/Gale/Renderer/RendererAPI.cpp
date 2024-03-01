#include "glpch.h"

#include "Gale/Renderer/RendererAPI.h"

#include "Gale/Vulkan/VulkanRenderer.h"
#include "Gale/DirectX/DirectXRenderer.h"

namespace Gale {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::Vulkan;

	RendererAPI* RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::API::None: { return nullptr; }
		case RendererAPI::API::Vulkan: { return new VulkanRenderer(); }
		case RendererAPI::API::DirectX: { return new DirectXRenderer(); }
		}

		GL_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}