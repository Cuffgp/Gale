#pragma once

#include "Gale/Renderer/Pipeline.h"
#include "Gale/Renderer/ShaderData.h"

#include "Gale/DirectX/DirectXDevice.h"
#include "Gale/DirectX/DirectXShader.h"

namespace Gale {

	class DirectXPipeline : public Pipeline
	{
	public:
		DirectXPipeline(const std::string& filepath);
		DirectXPipeline(const std::string& vertPath, const std::string& fragPath);
		~DirectXPipeline();

		void CreateRootSignature();
		void CreatePipeline();

		ID3D12PipelineState* GetState() { return m_PipelineState; }
		static ID3D12RootSignature* GetRootSignature() { return m_RootSignature; }
	private:
		std::string m_Filepath;
		Scope<DirectXShader> m_Shader;
		VertexInput m_Input;

		ID3D12PipelineState* m_PipelineState;
		static ID3D12RootSignature* m_RootSignature;
	};

}