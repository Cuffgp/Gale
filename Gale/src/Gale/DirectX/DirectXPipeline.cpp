#include "glpch.h"

#include "Gale/DirectX/DirectXPipeline.h"

namespace Gale {

	ID3D12RootSignature* DirectXPipeline::m_RootSignature = nullptr;

	DirectXPipeline::DirectXPipeline(const std::string& filepath)
	{
		m_Filepath = filepath;

		m_Shader = CreateScope<DirectXShader>(filepath);
		m_Input = m_Shader->GetVertexInput();

		CreateRootSignature();
		CreatePipeline();
	}

	DirectXPipeline::DirectXPipeline(const std::string& vertPath, const std::string& fragPath)
	{

	}

	DirectXPipeline::~DirectXPipeline()
	{
		m_PipelineState->Release();
	}

	void DirectXPipeline::CreateRootSignature()
	{
		auto device = DirectXDevice::Get().Device();

		ID3DBlob* signature;
		ID3DBlob* error;

		CD3DX12_DESCRIPTOR_RANGE ranges[1];
		ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);

		CD3DX12_ROOT_PARAMETER rootParameters[2]{};

		rootParameters[0].InitAsConstants(sizeof(glm::mat4) / 4, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
		//rootParameters[1].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_PIXEL);
		rootParameters[1].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);

		// Allow input layout and vertex shader and deny unnecessary access to certain pipeline stages.
		const D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_MESH_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_AMPLIFICATION_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
		// define root signature with transformation matrix
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(
			(UINT)std::size(rootParameters),
			rootParameters,
			0,
			nullptr,
			rootSignatureFlags);

		D3D12SerializeRootSignature(
			&rootSignatureDesc,
			D3D_ROOT_SIGNATURE_VERSION_1,
			&signature,
			&error
		);

		device->CreateRootSignature(
			0,
			signature->GetBufferPointer(),
			signature->GetBufferSize(),
			IID_PPV_ARGS(&m_RootSignature));

		signature->Release();
		if (error)
		{
			error->Release();
		}
	}

	void DirectXPipeline::CreatePipeline()
	{
		auto device = DirectXDevice::Get().Device();

		// Define the Graphics Pipeline
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs;

		for (auto& attribute : m_Input.GetAttributes())
		{
			D3D12_INPUT_ELEMENT_DESC inputElementDesc{};

			inputElementDesc.SemanticName = attribute.Name.c_str();
			inputElementDesc.SemanticIndex = 0;
			inputElementDesc.Format = Utils::ShaderDataTypeToDirectX(attribute.Type);
			inputElementDesc.InputSlot = 0;
			inputElementDesc.AlignedByteOffset = attribute.Offset;
			inputElementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			inputElementDesc.InstanceDataStepRate = 0;

			inputElementDescs.push_back(inputElementDesc);
		}

		// Shaders
		auto vertexShader = m_Shader->GetVertexBlob();
		auto pixelShader = m_Shader->GetPixelBlob();

		D3D12_SHADER_BYTECODE vsBytecode;
		vsBytecode.pShaderBytecode = vertexShader->GetBufferPointer();
		vsBytecode.BytecodeLength = vertexShader->GetBufferSize();

		D3D12_SHADER_BYTECODE psBytecode;
		psBytecode.pShaderBytecode = pixelShader->GetBufferPointer();
		psBytecode.BytecodeLength = pixelShader->GetBufferSize();

		// Rasterization
		D3D12_RASTERIZER_DESC rasterDesc;
		rasterDesc.FillMode = D3D12_FILL_MODE_SOLID;
		rasterDesc.CullMode = D3D12_CULL_MODE_BACK;
		rasterDesc.FrontCounterClockwise = FALSE;
		rasterDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		rasterDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		rasterDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		rasterDesc.DepthClipEnable = TRUE;
		rasterDesc.MultisampleEnable = FALSE;
		rasterDesc.AntialiasedLineEnable = FALSE;
		rasterDesc.ForcedSampleCount = 0;
		rasterDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		// Color/Blend
		D3D12_BLEND_DESC blendDesc;
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc = {
			FALSE,
			FALSE,
			D3D12_BLEND_ONE,
			D3D12_BLEND_ZERO,
			D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE,
			D3D12_BLEND_ZERO,
			D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL,
		};
		for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			blendDesc.RenderTarget[i] = defaultRenderTargetBlendDesc;

		// Depth/Stencil State
		D3D12_DEPTH_STENCIL_DESC depthDesc{};
		depthDesc.DepthEnable = TRUE;
		depthDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		depthDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		depthDesc.StencilEnable = FALSE;

		psoDesc.InputLayout = { inputElementDescs.data(), uint32_t(inputElementDescs.size()) };
		psoDesc.pRootSignature = m_RootSignature;
		psoDesc.VS = vsBytecode;
		psoDesc.PS = psBytecode;
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT); // a default depth stencil state
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;

		auto result = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PipelineState));
		if (result != S_OK)
		{
			GL_ERROR("Failed to create graphics pipeline state");
		}
	}



}