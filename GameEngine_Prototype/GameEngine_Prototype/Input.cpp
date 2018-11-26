#include "Input.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ApplicationManager.h"
#include "Time.h"

/*
Implement Input Manager with the following functionality:
- Initializes Input related settings.
- Tracks keyboard and mouse Input state changes every frame.
- Input API; public static functions for accessing key & mouse states:
e.g: Input::GetKeyDown(KeyCode someKey)
KeyDown: First frame the key is pressed.
KeyPressed: Key is pressed.
KeyUp: First frame the key is not pressed, after being pressed
GetMousePos: returns a vec2 for the position of the mouse on the screen.
GetMouseDelta: returns a vec2 for the distance the mouse has traveled last frame.
*/

Input::Input() 
{
	//keys[32].isEnabled = true; // enables spacebar only
	//keys[32].function = testFunction;
}
Input::~Input() {}

Input* Input::CreateManager()
{
	std::cout << "Creating Input Manager." << std::endl;
	Input* instance = &Input::getInstance();
	instance->Init();
	return instance;
}

void Input::Init()
{
	std::cout << "Initializing Input." << std::endl;
	glfwSetCursorPosCallback(ApplicationManager::APP_WINDOW, INPUT_MOUSE_CALLBACK);
	glfwSetMouseButtonCallback(ApplicationManager::APP_WINDOW, INPUT_MOUSE_BUTTON_CALLBACK);
	glfwSetScrollCallback(ApplicationManager::APP_WINDOW, INPUT_SCROLL_CALLBACK);
	showCursor(isCursorEnabled);
	isInitialized = true;
}

void Input::UpdateInput()
{
	checkKeyInputs();
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
	xScrollOffset = xoffset;
	yScrollOffset = yoffset;
}

void Input::_mouse_button_callback(int button, int action, int mods)
{
	for (int i = 0; i < sizeof(mouse) / sizeof(MouseButtonPressed); i++)
	{
		mouse[i].wasPressed = mouse[i].isButtonPressed;

		if (glfwGetMouseButton(ApplicationManager::getInstance().APP_WINDOW, i) == GLFW_PRESS 
			&& mouse[i].isButtonPressed == false)
		{
			//mouse[i].timeButtonDown = glfwGetTime();
			mouse[i].isButtonPressed = true;
			std::cout << "mouse button number " << i << " was pressed!" << std::endl;
		}
		else if (mouse[i].isButtonPressed == true)
		{
			//mouse[i].timeButtonUp = glfwGetTime();
			mouse[i].isButtonPressed = false;
			std::cout << "mouse button number " << i << " was released!" << std::endl;
			//std::cout << "mouse button press delta == " << timeMouseButtonReleasedDelta(i);
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

//double Input::GetMouseButtonTimeDelta(int glfw_mouse_button)
//{
//	return mouse[glfw_mouse_button].timeButtonUp 
//		- mouse[glfw_mouse_button].timeButtonDown;
//}

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
	return xScrollOffset;
}

double Input::GetScrollOffsetY()
{
	return yScrollOffset;
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


//void Input::keyPressed(int glfw_key)
//{
//	//keys[glfw_key].timeKeyDown = glfwGetTime(); // TODO: Should we used TimeManager?
//	keys[glfw_key].isPressed = true;
//}
//
//void Input::keyReleased(int glfw_key)
//{
//	//keys[glfw_key].timeKeyUp = glfwGetTime();
//	keys[glfw_key].isPressed = false;
//}

//void Input::timeKeyDown(int glfw_key, double glfw_get_time)
//{
//	//keys[glfw_key].timeKeyDown = glfw_get_time;
//}
//
//void Input::timeKeyUp(int glfw_key, double glfw_get_time)
//{
//	//keys[glfw_key].timeKeyUp = glfw_get_time;
//}
//
//double Input::timeKeyDown(int glfw_key)
//{
//	//return keys[glfw_key].timeKeyDown;
//}
//
//double Input::timeKeyUp(int glfw_key)
//{
//	//return keys[glfw_key].timeKeyUp;
//}

//double Input::GetKeyTimeDelta(int glfw_key)
//{
//	// If negative returned key is still pressed.
//	// Use to determine how long key has been held.
//	//return keys[glfw_key].timeKeyUp - keys[glfw_key].timeKeyDown;
//}

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
	for (int i = 0; i < 350; i++) // Might be inefficient
	{
		keys[i].wasPressed = keys[i].isPressed;

		if (glfwGetKey(ApplicationManager::APP_WINDOW, i) == GLFW_PRESS)
		{
			//if (keys[i].isPressed == false) keys[i].timeKeyDown = glfwGetTime();
			keys[i].isPressed = true;
		}
		else if (keys[i].isPressed == true)
		{
			//keys[i].timeKeyDown = glfwGetTime();
			keys[i].isPressed = false;
		}
	}
}

bool Input::getKeyDown(int glfw_key)
{
	return keys[glfw_key].wasPressed == false
		&& keys[glfw_key].isPressed == true;}

bool Input::getKey(int glfw_key)
{
	return keys[glfw_key].isPressed;
}

bool Input::getKeyUp(int glfw_key)
{
	return keys[glfw_key].wasPressed == true
		&& keys[glfw_key].isPressed == false;
}

bool Input::getMouseButtonDown(int glfw_mouse_button)
{
	return mouse[glfw_mouse_button].wasPressed == false
		&& mouse[glfw_mouse_button].isButtonPressed == true;
}

bool Input::getMouseButton(int glfw_mouse_button)
{
	return mouse[glfw_mouse_button].isButtonPressed;
}

bool Input::getMouseButtonUp(int glfw_mouse_button)
{
	return mouse[glfw_mouse_button].wasPressed == true
		&& mouse[glfw_mouse_button].isButtonPressed == false;
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
	return xDeltaPos;
}

double Input::getDeltaPosY()
{
	return yDeltaPos;
}

glm::vec2 Input::getMousePos()
{
	return glm::vec2(xPos, yPos);
}

glm::vec2 Input::getMouseDelta()
{
	return glm::vec2(xDeltaPos, yDeltaPos);
}

void Input::clearMouseDelta()
{
	xDeltaPos = 0.0f;
	yDeltaPos = 0.0f;
}

void Input::showCursor(bool enable)
{
	if (enable)
		glfwSetInputMode(ApplicationManager::APP_WINDOW, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else glfwSetInputMode(ApplicationManager::APP_WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// Toggles cursor off or on, returns new state of toggle.
bool Input::toggleCursor()
{
	isCursorEnabled = !isCursorEnabled;
	showCursor(isCursorEnabled);
	return isCursorEnabled;
}
