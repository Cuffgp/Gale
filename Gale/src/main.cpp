#include "GLFW/glfw3.h"

#include <iostream>

int main()
{
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