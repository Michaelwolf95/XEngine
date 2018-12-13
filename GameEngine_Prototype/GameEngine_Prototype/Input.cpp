#include "Input.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ApplicationManager.h"
#include "Time.h"

Input::Input() 
{
	xPos = 0.0f;
	yPos = xPos;
	xPosLast = 0.0f;
	yPosLast = xPosLast;
	xScrollOffset = 0.0f;
	yScrollOffset = 0.0f;
	firstMouse = true;
	mouseIdle = true;
	isCursorEnabled = true;
	mouse[10] = { false };
	keys[350] = { false };
}

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
	glfwSetMouseButtonCallback(ApplicationManager::APP_WINDOW, INPUT_MOUSE_BUTTON_CALLBACK);
	glfwSetScrollCallback(ApplicationManager::APP_WINDOW, INPUT_SCROLL_CALLBACK);
	showCursor(isCursorEnabled);
	isInitialized = true;
}

void Input::UpdateInput()
{
	glfwGetCursorPos(ApplicationManager::getInstance().APP_WINDOW, &xPos, &yPos);
	checkKeyInputs();

	mouseIdle = true;
}

void Input::_mouse_callback(double xpos, double ypos)
{
	if (firstMouse)
	{
		xPos = xpos; // TODO: Why doesn't applying a multiplier affect sensitivity?
		yPos = ypos; 
		xPosLast = xPos;
		yPosLast = yPos;
		firstMouse = false;
		mouseIdle = true;
	}

	xPos = xpos;
	yPos = ypos;	
	mouseIdle = false;
}

void Input::_scroll_callback(double xoffset, double yoffset)
{
	xScrollOffset = xoffset;
	yScrollOffset = yoffset;
}

void Input::_mouse_button_callback(int button, int action, int mods)
{
	for (int i = 0; i < m_arr_sz; i++)
	{
		mouse[i].wasPressed = mouse[i].isPressed;

		if (glfwGetMouseButton(ApplicationManager::getInstance().APP_WINDOW, i) 
			== GLFW_PRESS && mouse[i].isPressed == false)
		{
			mouse[i].isPressed = true;
		}
		else if (mouse[i].isPressed == true)
		{
			mouse[i].isPressed = false;
		}
	}
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

void INPUT_MOUSE_BUTTON_CALLBACK(GLFWwindow * window, int button, int action, int mods)
{
	Input* instance = &Input::getInstance();
	instance->_mouse_button_callback(button, action, mods);
}

glm::vec2 Input::GetMousePos()
{
	return Input::getInstance().getMousePos();
}

glm::vec2 Input::GetMouseDelta()
{
	return Input::getInstance().getMouseDelta();
}

bool Input::GetMouseButtonDown(int glfw_mouse_button)
{
	return Input::getInstance().getMouseButtonDown(glfw_mouse_button);
}

bool Input::GetMouseButton(int glfw_mouse_button)
{
	return Input::getInstance().getMouseButton(glfw_mouse_button);
}

bool Input::GetMouseButtonUp(int glfw_mouse_button)
{
	return Input::getInstance().getMouseButtonUp(glfw_mouse_button);
}

double Input::GetScrollOffsetX()
{
	return Input::getInstance().xScrollOffset;
}

double Input::GetScrollOffsetY()
{
	return Input::getInstance().yScrollOffset;
}

double Input::GetMousePosX()
{
	return Input::getInstance().getMousePosX();
}

double Input::GetMousePosY()
{
	return Input::getInstance().getMousePosY();
}

double Input::GetDeltaPosX()
{
	return Input::getInstance().getDeltaPosX();
}

double Input::GetDeltaPosY()
{
	return Input::getInstance().getDeltaPosY();
}

void Input::ShowCursor(bool enable)
{
	Input::getInstance().showCursor(enable);
}

void Input::ToggleCursor()
{
	Input::getInstance().toggleCursor();
}

bool Input::GetKeyDown(int glfw_key)
{
	return Input::getInstance().getKeyDown(glfw_key);
}

bool Input::GetKey(int glfw_key)
{
	return Input::getInstance().getKey(glfw_key);
}

bool Input::GetKeyUp(int glfw_key)
{
	return Input::getInstance().getKeyUp(glfw_key);
}

void Input::checkKeyInputs()
{
	for (int i = k_arr_start; i < k_arr_sz; i++) // Might be inefficient
	{
		keys[i].wasPressed = keys[i].isPressed;

		if (glfwGetKey(ApplicationManager::APP_WINDOW, i) == GLFW_PRESS)
		{
			keys[i].isPressed = true;
		}
		else if (keys[i].isPressed == true)
		{
			keys[i].isPressed = false;
		}
		// Skips a large set of unused keys. Saves a lot of CPU time.
		if (i == 96) i = 255; 
	}	

}

bool Input::getKeyDown(int glfw_key)
{
	validateKeyInputValue(glfw_key);
	return keys[glfw_key].wasPressed == false
		&& keys[glfw_key].isPressed == true;}

bool Input::getKey(int glfw_key)
{
	validateKeyInputValue(glfw_key);
	return keys[glfw_key].isPressed;
}

bool Input::getKeyUp(int glfw_key)
{
	validateKeyInputValue(glfw_key);
	return keys[glfw_key].wasPressed == true
		&& keys[glfw_key].isPressed == false;
}

bool Input::getMouseButtonDown(int glfw_mouse_button)
{
	validateMouseInputValue(glfw_mouse_button);
	return mouse[glfw_mouse_button].wasPressed == false
		&& mouse[glfw_mouse_button].isPressed == true;
}

bool Input::getMouseButton(int glfw_mouse_button)
{
	validateMouseInputValue(glfw_mouse_button);
	return mouse[glfw_mouse_button].isPressed;
}

bool Input::getMouseButtonUp(int glfw_mouse_button)
{
	validateMouseInputValue(glfw_mouse_button);
	return mouse[glfw_mouse_button].wasPressed == true
		&& mouse[glfw_mouse_button].isPressed == false;
}

bool Input::getMousePosX()
{
	return xPos;
}

bool Input::getMousePosY()
{
	return yPos;
}

double Input::getDeltaPosX()
{
	return getDelta(xPos, xPosLast);
}

double Input::getDeltaPosY()
{
	return -getDelta(yPos, yPosLast);
}

double Input::getDelta(double &pos, double &lastPos)
{
	double delta = lastPos - pos;
	mouseIdle = true;
	lastPos = pos;
	return delta;
}

glm::vec2 Input::getMousePos()
{
	return glm::vec2(xPos, yPos);
}

glm::vec2 Input::getMouseDelta()
{
	return glm::vec2(getDeltaPosX(), getDeltaPosY());
}

void Input::showCursor(bool enable)
{
	if (isCursorEnabled = enable) glfwSetInputMode(ApplicationManager::APP_WINDOW,
		GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else glfwSetInputMode(ApplicationManager::APP_WINDOW, 
		GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// Toggles cursor off or on, returns new state of toggle.
bool Input::toggleCursor()
{
	showCursor(!isCursorEnabled);
	return isCursorEnabled;
}

void Input::validateMouseInputValue(int glfw_mouse_button)
{
	if ((glfw_mouse_button < 0) | (glfw_mouse_button >= m_arr_sz))
	{
		printf("Throw mouse input error here!");
		// TODO: Error handling code. Error handling class?
	}
}

void Input::validateKeyInputValue(int glfw_key)
{
	if ((glfw_key < 0) | (glfw_key >= k_arr_sz))
	{
		printf("Throw key input error here! glfw_key == %i", glfw_key);
		// TODO: Error handling code. Error handling class?
	}
}