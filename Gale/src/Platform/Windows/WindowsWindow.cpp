#include "glpch.h"
#include "WindowsWindow.h"

#include "Gale/Events/ApplicationEvent.h"
#include "Gale/Events/MouseEvent.h"
#include "Gale/Events/KeyEvent.h"

namespace Gale {

	WindowsWindow::WindowsWindow()
	{

		m_Data.Title = "Hello Window";
		m_Data.Width = 800;
		m_Data.Height = 600;

		if (!glfwInit())
			GL_ERROR("Could not init GLFW!");

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		m_Window = glfwCreateWindow(800, 600, "Hello Window", NULL, NULL);
		GL_INFO("Window Created");

		glfwSetWindowUserPointer(m_Window, &m_Data);

		// Set GLFW Callbacks

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwTerminate();
	}

	void WindowsWindow::PollEvents()
	{
		glfwPollEvents();
	}

}
