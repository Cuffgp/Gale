#include "glpch.h"

#include "Application.h"
#include "Platform/Windows/Input.h"

namespace Gale {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;

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
			float time = m_Window->GetTime();
			m_Timestep.Set(time - m_LastFrameTime);
			m_LastFrameTime = time;

			//GL_INFO("Framerate: {}", 1 / m_Timestep.GetSeconds());

			m_Window->PollEvents();
			if (!m_minimized)
				OnUpdate(m_Timestep);
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

	void Application::OnUpdate(Timestep ts)
	{

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