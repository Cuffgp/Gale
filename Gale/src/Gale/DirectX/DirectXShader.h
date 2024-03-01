#pragma once

#include "Gale/Renderer/ShaderData.h"
#include "DirectXDevice.h"

namespace Gale {

	class DirectXShader
	{
	public:
		DirectXShader(const std::string& filepath);
		DirectXShader(const std::string& vertPath, const std::string& pixelPath);
		~DirectXShader();

		ID3DBlob* GetVertexBlob() { return m_VertexShader; }
		ID3DBlob* GetPixelBlob() { return m_PixelShader; }

		VertexInput& GetVertexInput() { return m_VertexInput; }
	private:

		void Reflect(ID3DBlob* shader, ShaderStage stage);

		VertexInput m_VertexInput;

		ID3DBlob* m_VertexShader = nullptr;
		ID3DBlob* m_PixelShader = nullptr;
	};

}