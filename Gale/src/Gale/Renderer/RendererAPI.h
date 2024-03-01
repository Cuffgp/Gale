#pragma once

#include "glm/glm.hpp"

#include "Gale/Renderer/Pipeline.h"
#include "Gale/Renderer/VertexBuffer.h"
#include "Gale/Renderer/IndexBuffer.h"
#include "Gale/Renderer/UniformBuffer.h"
#include "Gale/Renderer/DescriptorSet.h"

namespace Gale {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, Vulkan, DirectX
		};
	public:
		virtual ~RendererAPI() = default;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void BeginSwapchainRendering(Ref<Pipeline> pipeline) = 0;
		virtual void EndSwapchainRendering() = 0;

		virtual void WaitIdle() = 0;
		virtual void OnWindowResize(uint32_t width, uint32_t height) = 0;

		virtual void BindPipeline(Ref<Pipeline> pipeline) = 0;
		virtual void BindVertexBuffer(Ref<VertexBuffer> vertexBuffer) = 0;
		virtual void BindIndexBuffer(Ref<IndexBuffer> indexBuffer) = 0;
		virtual void BindUniformBuffer(Ref<UniformBuffer> uniformBuffer) = 0;
		virtual void BindDescriptorSet(Ref<DescriptorSet> descriptorSet) = 0;

		virtual void DrawIndexed(uint32_t indexCount) = 0;

		virtual void SetTransform(glm::mat4 transform) = 0;

		virtual uint32_t FramesInFlight() = 0;
		virtual uint32_t CurrentFrameIndex() = 0;

		static API Current() { return s_API; }
		static void Set(RendererAPI::API api) { s_API = api; }
		static RendererAPI* Create();
	private:
		static API s_API;
	};


}