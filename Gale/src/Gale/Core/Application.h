#pragma once
#include "Gale/Core/Base.h"
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
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		static Application& Get() { return *s_Instance; }
		WindowsWindow& GetWindow() { return *m_Window; }

	private:

		static Application* s_Instance;

		Ref<WindowsWindow> m_Window;
		Scope<VulkanContext> m_Context;
	};

}