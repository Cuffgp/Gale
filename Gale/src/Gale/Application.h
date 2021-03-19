#pragma once
#include "Gale/Base.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Platform/Vulkan/VulkanContext.h"

namespace Gale {

	class Application 
	{
	public:
		Application();
		~Application();

		void Run();
	private:
		Scope<WindowsWindow> m_Window;
		Scope<VulkanContext> m_Context;
	};

}