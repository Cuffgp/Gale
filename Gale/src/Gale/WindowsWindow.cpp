#include "glpch.h"
#include "WindowsWindow.h"

namespace Gale {

	WindowsWindow::WindowsWindow()
	{
		if (!glfwInit())
			GL_ERROR("Could not init GLFW!");

		m_Window = glfwCreateWindow(800, 600, "Hello Window", NULL, NULL);
		GL_INFO("Window Created");
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwTerminate();
	}

	void WindowsWindow::Run()
	{
		while (!glfwWindowShouldClose(m_Window))
		{
			glfwPollEvents();
		}
	}

}
