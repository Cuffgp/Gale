#include "glpch.h"
#include "Application.h"

namespace Gale {

	Application::Application()
	{
		m_Window = CreateScope<WindowsWindow>();
		m_Graphics = CreateRef<VulkanGraphics>(m_Window->GetWindowPointer());
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (!m_Window->ShouldClose())
		{
			m_Window->PollEvents();
			m_Graphics->drawFrame();
		}

		m_Graphics->end();
	}

}