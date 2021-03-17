#include "spdlog/spdlog.h"
#include "GLFW/glfw3.h"

#include "vulkan/vulkan.h"

#include <iostream>

int main()
{
	spdlog::info("Welcome to spdlog!");
	spdlog::error("Some error message with arg: {}", 1);

	GLFWwindow* window;

	if (!glfwInit())
		exit(EXIT_FAILURE);

	window = glfwCreateWindow(800, 600, "Simple example", NULL, NULL);

    while (!glfwWindowShouldClose(window))
    {
		glfwPollEvents();
    }

	std::cout << "Hello WOrld!" << std::endl;

	glfwTerminate();
}