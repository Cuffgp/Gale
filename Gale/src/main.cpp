#include "glpch.h"

#include "Gale/Core/Application.h"

int main()
{
	Gale::Log::Init();

	Gale::Application* App = new Gale::Application();

	App->Run();

	delete App;

}