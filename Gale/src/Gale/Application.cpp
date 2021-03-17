#include "glpch.h"
#include "Application.h"

namespace Gale {

	Application::Application()
	{
		m_Window = CreateScope<WindowsWindow>();
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (!m_Window->ShouldClose())
		{
			m_Window->PollEvents();
		}
	}

}