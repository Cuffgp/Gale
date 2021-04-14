#include "glpch.h"

#include "Application.h"


namespace Gale {

	Application::Application()
	{
		m_Window = CreateRef<WindowsWindow>();
		m_Window->SetEventCallback(GL_BIND_EVENT_FN(Application::OnEvent));

		m_Context = CreateScope<VulkanContext>(m_Window);
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (!m_Window->ShouldClose())
		{
			m_Window->PollEvents();
			if (!m_minimized)
				m_Context->drawFrame();
		}

		m_Context->waitIdle();

	}

	void Application::OnEvent(Event& e)
	{
		//GL_INFO("Event: {}", e.ToString());

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
		uint32_t width = e.GetWidth();
		uint32_t height = e.GetHeight();

		if (width == 0 || height == 0)
		{
			m_minimized = true;
			return false;
		}

		m_minimized = false;
		m_Context->onWindowResize(width, height);

		return false;
	}

}