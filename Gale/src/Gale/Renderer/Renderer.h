#pragma once

#include "Gale/Renderer/RendererAPI.h"

namespace Gale {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		static inline RendererAPI& Get() { return *s_Renderer; }

		static void BeginFrame(Ref<Pipeline> pipeline);
		static void EndFrame();

		static void WaitIdle();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BindPipeline(Ref<Pipeline> pipeline);
		static void BindVertexBuffer(Ref<VertexBuffer> vertexBuffer);
		static void BindIndexBuffer(Ref<IndexBuffer> indexBuffer);
		static void BindDescriptorSet(Ref<DescriptorSet> descriptorSet, uint32_t index);
		static void DrawIndexed(uint32_t indexCount);

		static void SetTransform(glm::mat4 transform);

	private:
		static RendererAPI* s_Renderer;
	};

}