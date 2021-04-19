#pragma once

#include "Gale/Core/Base.h"

namespace Gale {

	enum class ShaderDataType 
	{
		None = 0, Float, Float2, Float3
	};

	uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:  return 4;
			case ShaderDataType::Float2: return 4*2;
			case ShaderDataType::Float3: return 4*3;
		}
		GL_ERROR("Unknown shader data type");
		return 0;
	}

	struct BufferElement
	{
		BufferElement() {}

		BufferElement(ShaderDataType type, std::string name)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0)
		{
		}

		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout() {}

		VertexBufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
		}

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		void calculateOffsetAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;

			for (BufferElement& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer 
	{
	
	};

}