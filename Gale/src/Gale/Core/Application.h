#pragma once
#include "Gale/Core/Base.h"
#include "Gale/Core/Timestep.h"
#include "Gale/Events/Event.h"
#include "Gale/Events/ApplicationEvent.h"


#include "Platform/Windows/WindowsWindow.h"
#include "Platform/Vulkan/VulkanContext.h"

namespace Gale {

	class Application 
	{
	public:
		Application();
		~Application();

		void Run();
		void OnEvent(Event& e);
		void OnUpdate(Timestep ts);

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		static Application& Get() { return *s_Instance; }
		WindowsWindow& GetWindow() { return *m_Window; }

	private:
		Ref<WindowsWindow> m_Window;
		Scope<VulkanContext> m_Context;

		static Application* s_Instance;
		bool m_running = true;
		bool m_minimized = false;

		Timestep m_Timestep;
		float m_LastFrameTime = 0.0f;

	};

}