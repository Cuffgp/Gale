#include "Gale/Core/Base.h"
#include "Gale/Core/Window.h"

#include "Gale/Renderer/RendererAPI.h"

#include "Gale/Renderer/Pipeline.h"
#include "Gale/Renderer/VertexBuffer.h"
#include "Gale/Renderer/IndexBuffer.h"
#include "Gale/Renderer/UniformBuffer.h"
#include "Gale/Renderer/DescriptorSet.h"

namespace Gale {
	
	class Application
	{
	public:
		Application();
		~Application();

		void Run();
		void OnWindowResize(uint32_t width, uint32_t height);

		inline Window& GetWindow() { return *m_Window; }
		static inline Application& Get() { return *s_Instance; }
	private:
		Scope<Window> m_Window;
		float m_LastTime = 0;


		Ref<Pipeline> m_Pipeline;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<UniformBuffer> m_UniformBuffer;
		Ref<DescriptorSet> m_ConstantSet;

		static Application* s_Instance;
	};

	Application* CreateApplication();
}