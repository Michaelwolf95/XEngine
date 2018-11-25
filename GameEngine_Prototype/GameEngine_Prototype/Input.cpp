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
	keys[32].isEnabled = true; // enables spacebar only
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
	glfwSetScrollCallback(ApplicationManager::APP_WINDOW, INPUT_SCROLL_CALLBACK);
	glfwSetInputMode(ApplicationManager::APP_WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
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

	//xDeltaPos *= sensitivity;
	//yDeltaPos *= sensitivity;
	//
	//yaw += xDeltaPos;
	//pitch += yDeltaPos;

	//if (pitch > upperPitchMax)
	//	pitch = upperPitchMax;
	//if (pitch < lowerPitchMax)
	//	pitch = lowerPitchMax;
	//glm::vec3 front;
	//front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	//front.y = sin(glm::radians(pitch));
	//front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	
}
void Input::_scroll_callback(double xoffset, double yoffset)
{
	xScrollOffset = xoffset;
	yScrollOffset = yoffset;
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

glm::vec2 Input::GetMousePos()
{
	return glm::vec2(xPos, yPos);
}

glm::vec2 Input::GetMouseDelta()
{
	return glm::vec2(xDeltaPos, yDeltaPos);
}

void Input::checkKeyInputs()
{
	//glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
	//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	//float deltaMove = moveSpeed * Time::deltaTime;

	for (int i = 0; i < 350; i++)
	{
		if (glfwGetKey(ApplicationManager::APP_WINDOW, i) == GLFW_PRESS && keys[i].isEnabled == true)
		{
			if (keys[i].isKeyPressed == false) keys[i].timeKeyDown = glfwGetTime();
			keys[i].isKeyPressed = true;
		}
		else if (keys[i].isKeyPressed == true)
		{
			keys[i].timeKeyDown = glfwGetTime();
			keys[i].isKeyPressed = false;
		}
	}
	for (int i = 0; i < 350; i++)
	{
		if (keys[i].isKeyPressed == true)
		{
			std::cout << "key number " << i << " was marked true" << std::endl;
			keys[i].function();
		}
	}
}

void Input::keyPressed(int glfw_key)
{
	keys[glfw_key].timeKeyDown = glfwGetTime(); // TODO: Should we used TimeManager?
	keys[glfw_key].isKeyPressed = true;
}

void Input::keyReleased(int glfw_key)
{
	keys[glfw_key].timeKeyUp = glfwGetTime();
	keys[glfw_key].isKeyPressed = false;
}

bool Input::isKeyPressed(int glfw_key)
{
	return keys[glfw_key].isKeyPressed;
}

void Input::keyEnabled(int glfw_key, bool state)
{
	keys[glfw_key].isEnabled = state;
}

bool Input::isKeyEnabled(int glfw_key)
{
	return keys[glfw_key].isEnabled;
}

void Input::timeKeyDown(int glfw_key, double glfw_get_time)
{
	keys[glfw_key].timeKeyDown = glfw_get_time;
}

void Input::timeKeyUp(int glfw_key, double glfw_get_time)
{
	keys[glfw_key].timeKeyUp = glfw_get_time;
}

double Input::timeKeyDown(int glfw_key)
{
	return keys[glfw_key].timeKeyDown;
}

double Input::timeKeyUp(int glfw_key)
{
	return keys[glfw_key].timeKeyUp;
}

double Input::timeKeyPressReleaseDelta(int glfw_key)
{
	// If negative returned key is still pressed.
	// Use to determine how long key has been held.
	return keys[glfw_key].timeKeyUp - keys[glfw_key].timeKeyDown;
}

void Input::setKeyFunction(int glfw_key, std::function<void()> func)
{
	keys[glfw_key].function = func;
}

void Input::test()
{
	std::cout << "testing!" << std::endl;
}
