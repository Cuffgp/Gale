#include "glpch.h"

#include "Gale/DirectX/DirectXShader.h"

#include <dxcapi.h>
#include <d3d12shader.h>

namespace Gale {

	namespace Utils {

		static ShaderDataType DXReflectToShaderType(D3D_REGISTER_COMPONENT_TYPE type, byte mask)
		{
			if (type == D3D_REGISTER_COMPONENT_FLOAT32)
			{
				if (mask == 0b1) return ShaderDataType::Float;
				if (mask == 0b11) return ShaderDataType::Float2;
				if (mask == 0b111) return ShaderDataType::Float3;
				if (mask == 0b1111) return ShaderDataType::Float4;
			}
			GL_ASSERT(false, "Unknown shader data type");
			return ShaderDataType::None;
		}

	}

	DirectXShader::DirectXShader(const std::string& filepath)
	{
		uint32_t compileFlags = 0;
		if(DirectXDevice::m_Validation)
			compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

		std::string shaderPath = filepath + ".hlsl";
		std::wstring wPath(shaderPath.begin(), shaderPath.end());

		ID3DBlob* errorMessage;
		HRESULT result;

		result = D3DCompileFromFile(wPath.c_str(), nullptr, nullptr, "VSMain", "vs_5_1", compileFlags, 0, &m_VertexShader, &errorMessage);
		if (result != S_OK)
		{
			std::string errorMessageString(static_cast<const char*>(errorMessage->GetBufferPointer()), errorMessage->GetBufferSize());

			GL_ERROR("Vertex Error: {}", errorMessageString);
			GL_ASSERT(false, "");
		}

		result = D3DCompileFromFile(wPath.c_str(), nullptr, nullptr, "PSMain", "ps_5_1", compileFlags, 0, &m_PixelShader, &errorMessage);
		if (result != S_OK)
		{
			std::string errorMessageString(static_cast<const char*>(errorMessage->GetBufferPointer()), errorMessage->GetBufferSize());

			GL_ERROR("Fragment: {}", errorMessageString);
		}

		Reflect(m_VertexShader, ShaderStage::Vertex);
		Reflect(m_PixelShader, ShaderStage::Fragment);
	}

	DirectXShader::DirectXShader(const std::string& vertPath, const std::string& pixelPath)
	{

	}

	DirectXShader::~DirectXShader()
	{
		m_VertexShader->Release();
		m_PixelShader->Release();
	}

	void DirectXShader::Reflect(ID3DBlob* shader, ShaderStage stage)
	{
		ID3D12ShaderReflection* ShaderReflection;
		D3DReflect(shader->GetBufferPointer(), shader->GetBufferSize(), IID_ID3D12ShaderReflection, (void**)&ShaderReflection);

		D3D12_SHADER_DESC shaderDesc{};
		ShaderReflection->GetDesc(&shaderDesc);

		if (stage == ShaderStage::Vertex)
		{
			for (int index = 0; index < shaderDesc.InputParameters; index++)
			{

				D3D12_SIGNATURE_PARAMETER_DESC signatureParameterDesc{};
				ShaderReflection->GetInputParameterDesc(index, &signatureParameterDesc);

				std::string name = signatureParameterDesc.SemanticName;
				D3D_REGISTER_COMPONENT_TYPE type = signatureParameterDesc.ComponentType;
				byte mask = signatureParameterDesc.Mask;
				uint32_t location = signatureParameterDesc.Register;

				m_VertexInput.PushAttribute(VertexInputAttribute(name, Utils::DXReflectToShaderType(type, mask), location));
			}
			// Sort the vertex inputs by location
			std::sort(m_VertexInput.begin(), m_VertexInput.end(),
				[](const VertexInputAttribute& lhs, const VertexInputAttribute& rhs) {
					return lhs.Location < rhs.Location;
				});
			m_VertexInput.CalculateOffsetAndStride();
		}

		for (int i = 0; i < shaderDesc.ConstantBuffers; i++)
		{
			ID3D12ShaderReflectionConstantBuffer* constantBuffer = ShaderReflection->GetConstantBufferByIndex(i);

			//shaderDesc.BoundResources

			for (int j = 0; j < shaderDesc.BoundResources; j++)
			{
				ID3D12ShaderReflectionVariable* variable = constantBuffer->GetVariableByIndex(j);
				D3D12_SHADER_VARIABLE_DESC variableDesc;
				variable->GetDesc(&variableDesc);
				GL_INFO("");
			}

			D3D12_SHADER_BUFFER_DESC bufferDesc;
			constantBuffer->GetDesc(&bufferDesc);
			constantBuffer->GetVariableByIndex(0);

			auto name = bufferDesc.Name;
			auto size = bufferDesc.Size;
			auto type = bufferDesc.Type;

			GL_INFO("Constant {}, size {}", name, size);
		}

		ShaderReflection->Release();
	}

}