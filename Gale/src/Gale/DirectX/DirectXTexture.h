#pragma once

#include "Gale/Renderer/Texture.h"

#include "Gale/DirectX/DirectX.h"

namespace Gale {

	class DirectXTexture : public Texture
	{
	public:
		DirectXTexture(std::string filepath);
		~DirectXTexture();

		virtual void Write(Ref<DescriptorSet> descriptorSet) override;
	private:

		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_Size;

		ID3D12Resource* m_Image;
	};

}