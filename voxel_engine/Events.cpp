#include <string.h>
#include "Events.h"

bool* Events::_keys;
unsigned int* Events::_frames;
unsigned int Events::_current_frame = 0;
float Events::deltaX = 0.0f;
float Events::deltaY = 0.0f;
float Events::x = 0.0f;
float Events::y = 0.0f;
bool Events::_cursor_locked = false;
bool Events::_cursor_started = false;

#define _MOUSE_BUTTONS  1024

void cursor_postion_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (Events::_cursor_started)
	{
		Events::deltaX += xPos - Events::x;
		Events::deltaY += yPos - Events::y;
	}
	else
	{
		Events::_cursor_started = true;
	}
	Events::x = xPos;
	Events::y = yPos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		Events::_keys[_MOUSE_BUTTONS + button] = true;
		Events::_frames[_MOUSE_BUTTONS + button] = Events::_current_frame;
	} else if (action == GLFW_RELEASE)
	{
		Events::_keys[_MOUSE_BUTTONS + button] = false;
		Events::_frames[_MOUSE_BUTTONS + button] = Events::_current_frame;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		Events::_keys[key] = true;
		Events::_frames[key] = Events::_current_frame;
	}
	else if (action == GLFW_RELEASE)
	{
		Events::_keys[key] = false;
		Events::_frames[key] = Events::_current_frame;
	}
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	Window::width = width;
	Window::height = height;
}

int Events::initialize()
{
	GLFWwindow* window = Window::window;
	_keys = new bool[1032];
	_frames = new unsigned int[1032];

	memset(_keys, false, 1032*sizeof(bool));
	memset(_frames, 0, 1032*sizeof(unsigned int));

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_postion_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);

	return 0;
}

void Events::pullEvents()
{
	_current_frame++;
	deltaX = 0.0f;
	deltaY = 0.0f;


	glfwPollEvents();
}

bool Events::isPressed(int keyCode)
{
	if (keyCode < 0 || keyCode > _MOUSE_BUTTONS)
	{
		return false;
	}

	return _keys[keyCode];
}

bool Events::justPressed(int keyCode)
{
	if (keyCode < 0 || keyCode > _MOUSE_BUTTONS)
	{
		return false;
	}

	return _keys[keyCode] && _frames[keyCode] == _current_frame;
}

bool Events::isClicked(int button)
{
	int index = _MOUSE_BUTTONS + button;
	return _keys[index];
}

bool Events::justClicked(int button)
{
	int index = _MOUSE_BUTTONS + button;
	return _keys[index] && _frames[index] == _current_frame;
}

void Events::toggleCursor()
{
	_cursor_locked = !_cursor_locked;
	Window::setCursorMode(_cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}
