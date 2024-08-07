#include "glpch.h"

#include "Gale/Core/Utilities.h"
#include "Gale/Core/Application.h"
#include "Gale/Renderer/Renderer.h"
#include "Gale/Renderer/Camera.h"
#include "Gale/Renderer/ObjLoader.h"

namespace Gale {
	
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;

		GL_INFO("Creating application");

		m_Window = CreateScope<Window>(1280, 720, "Gale Application");
		m_Window->SetResizeCallback(GL_BIND_FN(Application::OnWindowResize));

		Renderer::Init();

		float CubeVertices[] = {
			-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f
		};

		uint32_t CubeIndices[] = {
			0, 1, 2,  2, 3, 0,
			1, 5, 6,  6, 2, 1,
			5, 4, 7,  7, 6, 5,
			4, 0, 3,  3, 7, 4,
			3, 2, 6,  6, 7, 3,
			4, 5, 1,  1, 0, 4
		};

		float FaceColors[] = {
			1.f, 0.f, 0.f, 1.f,
			0.f, 1.f, 0.f, 1.f,
			0.f, 0.f, 1.f, 1.f,
			1.f, 0.f, 1.f, 1.f,
			0.f, 1.f, 1.f, 1.f,
			1.f, 1.f, 0.f, 1.f,
		};

		//auto vertexInput = VertexInput({
		//{ "Pos",   ShaderDataType::Float3, 0 }
		//	});

		auto vertexInput = VertexInput({
			{ "Pos",		ShaderDataType::Float3, 0 },
			{ "Normal",		ShaderDataType::Float3, 1 },
			{ "Uv",			ShaderDataType::Float2, 2 }
			});

		DescriptorMap descriptorMap;
		descriptorMap[0] = ShaderDescriptor(0, 0, ShaderStage::Fragment, ShaderDescriptorType::Uniform, 1);

		m_ConstantSet = DescriptorSet::Create(descriptorMap);

		DescriptorSetMap descriptorSetMap;
		descriptorSetMap[0] = descriptorMap;

		//m_VertexBuffer = VertexBuffer::Create(CubeVertices, 8 * 8 * sizeof(float), vertexInput);
		//m_IndexBuffer = IndexBuffer::Create(CubeIndices, 6 * 6 * sizeof(uint32_t));

		auto obj = ObjLoader("assets/mesh/flat_vase.obj");

		m_VertexBuffer = obj.GetVertexBuffer();
		m_IndexBuffer = obj.GetIndexBuffer();

		m_Texture = Texture::Create("assets/texture/statue.jpg");

		m_UniformBuffer = UniformBuffer::Create(6 * 4 * sizeof(float));
		m_UniformBuffer->SetData(FaceColors);

		m_UniformBuffer->Write(m_ConstantSet);

		m_Pipeline = Pipeline::Create("assets/shaders/uniform_shader", descriptorSetMap);

		GL_INFO("Index Count {}", m_IndexBuffer->GetIndexCount());

	}
	
	Application::~Application()
	{
		Renderer::Shutdown();
		GL_INFO("Destroying application");
	}

	void Application::Run()
	{
		std::array<glm::mat4, 2> viewProjection;

		Camera camera;

		while (!m_Window->ShouldClose())
		{
			float time = (float)glfwGetTime();
			float timestep = time - m_LastTime;
			m_LastTime = time;

			camera.OnUpdate(timestep);

			viewProjection[0] = camera.GetView();
			viewProjection[1] = camera.GetProjection();

			glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3{ 1, 0, 0 });
			glm::mat4 translation2 = glm::translate(glm::mat4(1.0f), glm::vec3{ -1, 0, 0 });
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), time, glm::vec3(1, 1, 1));
			glm::mat4 rotation2 = glm::rotate(glm::mat4(1.0f), time, glm::vec3(-0.8, -0.5, 1));

			m_Window->PollEvents();

			Renderer::BeginFrame(m_Pipeline);
			Renderer::BindVertexBuffer(m_VertexBuffer);
			Renderer::BindIndexBuffer(m_IndexBuffer);

			Renderer::BindDescriptorSet(m_ConstantSet, 0);

			Renderer::SetTransform(camera.GetProjectionView() * translation * rotation);
			Renderer::DrawIndexed(m_IndexBuffer->GetIndexCount());
			Renderer::SetTransform(camera.GetProjectionView() * translation2 * rotation2);
			Renderer::DrawIndexed(m_IndexBuffer->GetIndexCount());
			Renderer::EndFrame();
		}
	}

	void Application::OnWindowResize(uint32_t width, uint32_t height)
	{
		GL_INFO("Window resizing");
		Renderer::OnWindowResize(width, height);
	}
	
}