#include "glpch.h"

#include "Gale/Application.h"
#include "vulkan/vulkan.h"



int main()
{
	Gale::Log::Init();

	Gale::Application* App = new Gale::Application();

	App->Run();

	delete App;

}