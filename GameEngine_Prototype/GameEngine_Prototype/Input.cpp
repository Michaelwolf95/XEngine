#include "Input.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ApplicationManager.h"


Input::Input() {}

Input::~Input() {}

Input* Input::CreateManager()
{
	Input* instance = &Input::getInstance();
	instance->Init();

	return instance;
}

void Input::Init()
{
	glfwSetCursorPosCallback(ApplicationManager::APP_WINDOW, INPUT_MOUSE_CALLBACK);
	glfwSetScrollCallback(ApplicationManager::APP_WINDOW, INPUT_SCROLL_CALLBACK);
	isInitialized = true;
}

void Input::_mouse_callback(double xpos, double ypos)
{
	if (firstMouse)
	{
		xPos = xpos;
		yPos = ypos;
		firstMouse = false;
	}
	xDeltaPos = xpos - xPos; 
	yDeltaPos = yPos - ypos; // reversed since y-coordinates go from bottom to top
	xPos = xpos;
	yPos = ypos;
}
void Input::_scroll_callback(double xoffset, double yoffset)
{
	xOffset = xoffset;
	yOffset = yoffset;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void INPUT_MOUSE_CALLBACK(GLFWwindow* window, double xpos, double ypos)
{
	Input* instance = &Input::getInstance();
	instance->_mouse_callback(xpos, ypos);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void INPUT_SCROLL_CALLBACK(GLFWwindow* window, double xoffset, double yoffset)
{
	Input* instance = &Input::getInstance();
	instance->_scroll_callback(xoffset, yoffset);
}