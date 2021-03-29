#include "glpch.h"

#include "Application.h"

namespace Gale {

	Application::Application()
	{
		m_Window = CreateScope<WindowsWindow>();
		m_Window->SetEventCallback(GL_BIND_EVENT_FN(Application::OnEvent));

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

	void Application::OnEvent(Event& e)
	{
		GL_INFO("Event: {}", e.ToString());

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(GL_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(GL_BIND_EVENT_FN(Application::OnWindowResize));
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		return false;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		return false;
	}

}