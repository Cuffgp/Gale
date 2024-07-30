#pragma once

#include "Gale/Core/Base.h"

#include "Gale/Renderer/VertexBuffer.h"
#include "Gale/Renderer/IndexBuffer.h"

namespace Gale {

	class ObjLoader
	{
	public:
		struct Vertex
		{
			glm::vec3 Position = glm::vec3{0., 0., 0.};
			glm::vec3 Normal = glm::vec3{ 0., 0., 0. };
			glm::vec2 Uv = glm::vec2{ 0., 0.};
		};
	public:
		ObjLoader(std::string filepath);
		~ObjLoader();

		Ref<VertexBuffer> GetVertexBuffer() { return m_VertexBuffer; }
		Ref<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }
	private:
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
	};

}