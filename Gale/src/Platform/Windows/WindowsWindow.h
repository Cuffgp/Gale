#pragma once
#include <GLFW/glfw3.h>
#include "Gale/Events/Event.h"

namespace Gale {

	class WindowsWindow
	{
	public:
		WindowsWindow();
		~WindowsWindow();

		bool ShouldClose() { return glfwWindowShouldClose(m_Window); };
		void PollEvents();
		GLFWwindow* GetWindowPointer() { return m_Window; }
		void SetEventCallback(const std::function<void(Event&)>& callback) { m_Data.EventCallback = callback; }

	private:

		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			std::function<void(Event&)> EventCallback;
		};

		WindowData m_Data;
	};

}