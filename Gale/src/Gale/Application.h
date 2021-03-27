#pragma once
#include "Gale/Base.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Platform/Vulkan/VulkanGraphics.h"

namespace Gale {

	class Application 
	{
	public:
		Application();
		~Application();

		void Run();
	private:
		Scope<WindowsWindow> m_Window;
		Ref<VulkanGraphics> m_Graphics;
	};

}