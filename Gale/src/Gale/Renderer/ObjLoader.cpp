#include "glpch.h"

#include "Gale/Renderer/ObjLoader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader/tiny_obj_loader.h"

namespace Gale {

	ObjLoader::ObjLoader(std::string filepath)
	{

		tinyobj::ObjReaderConfig reader_config;
		//reader_config.mtl_search_path = "./"; // Path to material files

		tinyobj::ObjReader reader;

		if (!reader.ParseFromFile(filepath, reader_config)) 
		{
			if (!reader.Error().empty()) 
			{
				GL_ERROR("TinyObjReader: {}", reader.Error());
			}
		}

		if (!reader.Warning().empty()) 
		{
			GL_WARN("TinyObjReader: {}", reader.Warning());
		}

		auto& attrib = reader.GetAttrib();
		auto& shapes = reader.GetShapes();
		//auto& materials = reader.GetMaterials();

		GL_INFO("Number of shapes {}", shapes.size());

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) 
		{
			Vertex vertex;
			// Loop over faces(polygon)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) 
			{
				size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) 
				{
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					vertex.Position.x = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
					vertex.Position.y = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
					vertex.Position.z = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

					// Check if `normal_index` is zero or positive. negative = no normal data
					if (idx.normal_index >= 0) 
					{
						vertex.Normal.x = attrib.normals[3 * size_t(idx.normal_index) + 0];
						vertex.Normal.y = attrib.normals[3 * size_t(idx.normal_index) + 1];
						vertex.Normal.z = attrib.normals[3 * size_t(idx.normal_index) + 2];
					}

					// Check if `texcoord_index` is zero or positive. negative = no texcoord data
					if (idx.texcoord_index >= 0) 
					{
						vertex.Uv.x = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
						vertex.Uv.y = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
					}

					// Optional: vertex colors
					// tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
					// tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
					// tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];

					vertices.push_back(vertex);
					indices.push_back(indices.size());
				}
				index_offset += fv;

				// per-face material
				//shapes[s].mesh.material_ids[f];
			}
		}
		auto vertexInput = VertexInput({
			{ "Pos",   ShaderDataType::Float3, 0 },
			{ "Normal",   ShaderDataType::Float3, 0 },
			{ "Uv",   ShaderDataType::Float2, 0 }
			});

		m_VertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size(), vertexInput);
		m_IndexBuffer = IndexBuffer::Create(indices.data(), indices.size());
	}

	ObjLoader::~ObjLoader()
	{

	}
}