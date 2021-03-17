#include "glpch.h"

#include "vulkan/vulkan.h"

#include "Gale/Log.h"
#include "Gale/WindowsWindow.h"

int main()
{
	Gale::Log::Init();

	GL_TRACE("Hello Trace");
	GL_INFO("Hello Info");
	GL_WARN("Hello Warn");
	GL_ERROR("Hello Error");
	GL_FATAL("Hello Critical");

	Gale::Scope<Gale::WindowsWindow> Window = Gale::CreateScope<Gale::WindowsWindow>();

	Window->Run();

}