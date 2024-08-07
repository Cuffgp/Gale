#include "glpch.h"

#include "Gale/Renderer/Renderer.h"

namespace Gale {

	RendererAPI* Renderer::s_Renderer = nullptr;

	void Renderer::Init()
	{
		s_Renderer = RendererAPI::Create();
	}

	void Renderer::Shutdown()
	{
		s_Renderer->WaitIdle();
		delete s_Renderer;
	}

	void Renderer::BeginFrame(Ref<Pipeline> pipeline)
	{
		s_Renderer->BeginFrame();
		s_Renderer->BeginSwapchainRendering(pipeline);
		s_Renderer->BindPipeline(pipeline);
	}

	void Renderer::EndFrame()
	{
		s_Renderer->EndSwapchainRendering();
		s_Renderer->EndFrame();
	}

	void Renderer::WaitIdle()
	{
		s_Renderer->WaitIdle();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		s_Renderer->OnWindowResize(width, height);
	}

	void Renderer::BindPipeline(Ref<Pipeline> pipeline)
	{
		s_Renderer->BindPipeline(pipeline);
	}

	void Renderer::BindVertexBuffer(Ref<VertexBuffer> vertexBuffer)
	{
		s_Renderer->BindVertexBuffer(vertexBuffer);
	}

	void Renderer::BindIndexBuffer(Ref<IndexBuffer> indexBuffer)
	{
		s_Renderer->BindIndexBuffer(indexBuffer);
	}

	void Renderer::BindDescriptorSet(Ref<DescriptorSet> descriptorSet, uint32_t index)
	{
		s_Renderer->BindDescriptorSet(descriptorSet, index);
	}

	void Renderer::DrawIndexed(uint32_t indexCount)
	{
		s_Renderer->DrawIndexed(indexCount);
	}

	void Renderer::SetTransform(glm::mat4 transform)
	{
		s_Renderer->SetTransform(transform);
	}

}