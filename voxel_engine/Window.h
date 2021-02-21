#ifndef WINDOW_WINDOW_H_
#define WINDOW_WINDOW_H_
#define GLEW_STATIC
#define LIBPNG_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class GLFWwindow;

class Window
{
public:
	static GLFWwindow* window;
	static int width;
	static int height;

	static int initialize(int width, int height, const char* title);
	static void terminate();

	static void setCursorMode(int mode);

	static bool isShouldClose();
	static void shouldClose(bool state);

	static void swapBuffers();

	static void setRandomScreenColor();
};

#endif