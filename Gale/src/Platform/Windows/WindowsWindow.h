#pragma once
#include <GLFW/glfw3.h>


namespace Gale {

	class WindowsWindow
	{
	public:
		WindowsWindow();
		~WindowsWindow();

		bool ShouldClose() { return glfwWindowShouldClose(m_Window); };
		void PollEvents();
	private:
		GLFWwindow* m_Window;
	};

}