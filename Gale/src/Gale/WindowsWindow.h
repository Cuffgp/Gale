#pragma once
#include <GLFW/glfw3.h>


namespace Gale {

	class WindowsWindow
	{
	public:
		WindowsWindow();
		~WindowsWindow();

		void Run();
	private:
		GLFWwindow* m_Window;
	};

}