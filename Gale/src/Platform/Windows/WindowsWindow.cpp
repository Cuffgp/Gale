#include "glpch.h"
#include "WindowsWindow.h"

namespace Gale {

	WindowsWindow::WindowsWindow()
	{
		if (!glfwInit())
			GL_ERROR("Could not init GLFW!");

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		m_Window = glfwCreateWindow(800, 600, "Hello Window", NULL, NULL);
		GL_INFO("Window Created");
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
