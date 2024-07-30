#include "glpch.h"

#include "Gale/DirectX/DirectXDevice.h"
#include "Gale/DirectX/DirectXRenderer.h"
#include "Gale/DirectX/DirectXDescriptorSet.h"

#include "Gale/DirectX/DirectXTexture.h"
#include "Gale/DirectX/DirectXBuffer.h"

#include "stb_image/stb_image.h"

namespace Gale {

	DirectXTexture::DirectXTexture(std::string filepath)
	{
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load(filepath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		m_Size = texWidth * texHeight * 4;

		if (!pixels) 
		{
			GL_ASSERT(false, "Failed to load texture image {}", filepath);
		}

		auto device = DirectXDevice::Get().Device();

		m_Width = texWidth;
		m_Height = texHeight;

		D3D12_RESOURCE_DESC textureDesc = {};
		textureDesc.MipLevels = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Match the stb_image output format
		textureDesc.Width = m_Width;
		textureDesc.Height = m_Height;
		textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		textureDesc.DepthOrArraySize = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

		D3D12_HEAP_PROPERTIES heapProperties = {};
		heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

		device->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&m_Image));

		DirectXBuffer uploadBuffer(
			m_Size,
			D3D12_HEAP_TYPE_UPLOAD,
			D3D12_RESOURCE_STATE_GENERIC_READ);

		D3D12_SUBRESOURCE_DATA textureData = {};
		textureData.pData = pixels; // Pointer to the raw image data loaded with stb_image
		textureData.RowPitch = m_Width * 4; // Number of bytes per row (4 bytes per pixel for RGBA)
		textureData.SlicePitch = textureData.RowPitch * m_Height; // Total size of the image data

		auto commandList = DirectXDevice::Get().BeginSingleTimeCommands();

		UpdateSubresources(commandList, m_Image, uploadBuffer.GetBuffer(), 0, 0, 1, &textureData);

		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			m_Image,
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
		);
		commandList->ResourceBarrier(1, &barrier);
		DirectXDevice::Get().EndSingleTimeCommands(commandList);

		stbi_image_free(pixels);
	}

	DirectXTexture::~DirectXTexture()
	{
		m_Image->Release();
	}

	void DirectXTexture::Write(Ref<DescriptorSet> descriptorSet)
	{
		auto device = DirectXDevice::Get().Device();
		auto heap = DirectXRenderer::DescriptorHeap->SamplerHeap();

		// Assuming you have a descriptor heap for SRVs
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		// srvHeapHandle is the descriptor heap handle
		device->CreateShaderResourceView(m_Image, &srvDesc, heap->GetCPUDescriptorHandleForHeapStart());
	}

}