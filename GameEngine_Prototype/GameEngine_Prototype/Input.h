#pragma once
#include "Singleton.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <vector>
#include <functional>

//enum KeyCode
//{
//
//};

void INPUT_MOUSE_CALLBACK(GLFWwindow* window, double xpos, double ypos);
void INPUT_SCROLL_CALLBACK(GLFWwindow* window, double xoffset, double yoffset);
//TODO: Store mouse callback output every frame from 
/*
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
*/

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

class Input : public Singleton<Input>
{
public:
	struct KeyPressed
	{		
		bool isEnabled = false;
		bool isKeyPressed = false;
		double timeKeyDown = 0.0f; // keyDown
		double timeKeyUp = 0.0f; // keyUp
		std::function<void()> function;
	};

	friend class Singleton<Input>;
	Input();
public:
	double xPos; // TODO: Discuss why we should use 64bit double over 32bit float
	double yPos;
	double xDeltaPos;
	double yDeltaPos;
	//double yaw;
	//double pitch;
	double xScrollOffset;
	double yScrollOffset;
	bool firstMouse = true;
	void keyPressed(int glfw_key);
	void keyReleased(int glfw_key);
	bool isKeyPressed(int glfw_key);
	void keyEnabled(int glfw_key, bool state);
	bool isKeyEnabled(int glfw_key);
	void timeKeyDown(int glfw_key, double glfw_get_time);
	void timeKeyUp(int glfw_key, double glfw_get_time);
	double timeKeyDown(int glfw_key);
	double timeKeyUp(int glfw_key);
	double timeKeyPressReleaseDelta(int glfw_key);
	void setKeyFunction(int glfw_key, std::function<void()> func);
	// TODO: Input::GetButtonHold(KEY_CODE); // returns true or false


	// TODO: Symmetric array can be implemented that hold functions 
	// specific to each index of this array.
	// Developers may insert functions with API calls
	// void InsertKeyFunction(unsigned int GLFW_KEY_VALUE, (void*)functionToCallWhenKeyPressed());
	KeyPressed keys[350] = { false }; // tracks which keys are pressed
	//double sensitivity = 0.05f;
	//double upperPitchMax = 89.0f;
	//double lowerPitchMax = -89.0f;
	~Input();
	void _mouse_callback(double xpos, double ypos);
	void _scroll_callback(double xoffset, double yoffset);

	bool isInitialized = false;

	// Create static instance and configure manager
	static Input* CreateManager();

	// Init instance and setup GLFW, etc.
	void Init();

	// Update keys for each frame.
	void UpdateInput();

	// Returns mouse position as a vec2
	glm::vec2 GetMousePos();
	glm::vec2 GetMouseDelta();
	// TODO: Maybe return a container with values for each key pressed?
	void checkKeyInputs();
};

