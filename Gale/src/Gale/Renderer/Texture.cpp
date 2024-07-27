#include "glpch.h"

#include "Gale/Renderer/RendererAPI.h"
#include "Gale/Renderer/Texture.h"

#include "Gale/DirectX/DirectXTexture.h"
#include "Gale/Vulkan/VulkanTexture.h"


namespace Gale {

	Ref<Texture> Texture::Create(std::string filepath)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPI::API::None: { return nullptr; }
		case RendererAPI::API::Vulkan: { return CreateRef<VulkanTexture>(filepath); }
		case RendererAPI::API::DirectX: { return CreateRef<DirectXTexture>(filepath); }
		}
		GL_ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}

}