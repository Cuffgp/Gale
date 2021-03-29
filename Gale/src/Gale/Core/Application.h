#pragma once
#include "Gale/Core/Base.h"
#include "Gale/Events/Event.h"
#include "Gale/Events/ApplicationEvent.h"

#include "Platform/Windows/WindowsWindow.h"
#include "Platform/Vulkan/VulkanGraphics.h"

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

	private:
		Scope<WindowsWindow> m_Window;
		Ref<VulkanGraphics> m_Graphics;
	};

}