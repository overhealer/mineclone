#include "Window.h"

GLFWwindow* Window::window;
int Window::width = 0;
int Window::height = 0;


int Window::initialize(int width, int height, const char* title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);

	if (window == nullptr)
	{
		std::cerr << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	glViewport(0, 0, width, height);
	Window::width = width;
	Window::height = height;
	return 0;
}

void Window::terminate()
{
	glfwTerminate();
}

void Window::setCursorMode(int mode) {
	glfwSetInputMode(window, GLFW_CURSOR, mode);
}

bool Window::isShouldClose()
{
	return glfwWindowShouldClose(window);
}

void Window::shouldClose(bool state)
{
	glfwSetWindowShouldClose(window, state);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(window);
}

void Window::setRandomScreenColor()
{
	float randR = static_cast <float> (rand()) / static_cast <float>(RAND_MAX);
	float randG = static_cast <float> (rand()) / static_cast <float>(RAND_MAX);
	float randB = static_cast <float> (rand()) / static_cast <float>(RAND_MAX);
	glClearColor(randR, randG, randB, 1);
}
