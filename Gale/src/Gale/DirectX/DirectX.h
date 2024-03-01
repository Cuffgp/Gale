#pragma once

#include "Gale/Core/Base.h"

#include "Gale/Renderer/ShaderData.h"

#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
#include "d3dx12/d3dx12.h"

namespace Gale {

	namespace Utils {

		static DXGI_FORMAT ShaderDataTypeToDirectX(ShaderDataType type)
		{
			switch (type)
			{
				case ShaderDataType::Float:  return DXGI_FORMAT_R32_FLOAT;
				case ShaderDataType::Float2: return DXGI_FORMAT_R32G32_FLOAT;
				case ShaderDataType::Float3: return DXGI_FORMAT_R32G32B32_FLOAT;
				case ShaderDataType::Float4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
			GL_ASSERT(false, "Unknown shader data type");
			return DXGI_FORMAT_UNKNOWN;
		}

	}

}

