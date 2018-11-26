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
void INPUT_MOUSE_BUTTON_CALLBACK(GLFWwindow* window, int button, int action, int mods);
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
private:
	struct KeyPressed
	{		
		//bool isEnabled = false;
		bool isPressed = false;
		//double timeKeyDown = 0.0f; // keyDown
		//double timeKeyUp = 0.0f; // keyUp
		bool wasPressed = false;
		//std::function<void()> function; // keeping for reference
	};
	struct MouseButtonPressed
	{
		bool isButtonPressed = false;
		//double timeButtonDown = 0.0f;
		//double timeButtonUp = 0.0f;
		bool wasPressed = false;
	};
	friend class Singleton<Input>;

	bool isInitialized = false;
	double xPos; // TODO: Discuss why we should use 64bit double over 32bit float
	double yPos;
	double xDeltaPos;
	double yDeltaPos;
	double xScrollOffset;
	double yScrollOffset;
	bool firstMouse = true;
	MouseButtonPressed mouse[10] = { false };
	KeyPressed keys[350] = { false }; // tracks which keys are pressed
	Input();
	void checkKeyInputs();
	bool getKeyDown(int glfw_key);
	bool getKey(int glfw_key);
	bool getKeyUp(int glfw_key);
	bool getMouseButtonDown(int glfw_mouse_button);
	bool getMouseButton(int glfw_mouse_button);
	bool getMouseButtonUp(int glfw_mouse_button);
	bool getMousePosX();
	bool getMousePosY();
	double getDeltaPosX();
	double getDeltaPosY();

	
protected:
	// Init instance and setup GLFW, etc.
	void Init();

public:
	// Create static instance and configure manager
	static Input* CreateManager();
	// Update keys for each frame.
	void UpdateInput();
	~Input();
	/// Mouse inputs
	glm::vec2 GetMousePos(); // Returns mouse position as a vec2
	glm::vec2 GetMouseDelta();
	static bool GetMouseButtonDown(int glfw_mouse_button); //API
	static bool GetMouseButton(int glfw_mouse_button); //API
	static bool GetMouseButtonUp(int glfw_mouse_button); //API
	double GetScrollOffsetX();
	double GetScrollOffsetY();
	static double GetMousePosX();
	static double GetMousePosY();
	static double GetDeltaPosX();
	static double GetDeltaPosY();
	//double GetMouseButtonTimeDelta(int glfw_mouse_button); //API

	/// Keyboard inputs

	static bool GetKeyDown(int glfw_key); //API
	static bool GetKey(int glfw_key); //API
	static bool GetKeyUp(int glfw_key); //API	
	//void keyPressed(int glfw_key);
	//void keyReleased(int glfw_key);
	//void timeKeyDown(int glfw_key, double glfw_get_time);
	//void timeKeyUp(int glfw_key, double glfw_get_time);
	//double timeKeyDown(int glfw_key);
	//double timeKeyUp(int glfw_key);
	//double GetKeyTimeDelta(int glfw_key); //API
	//void keyEnabled(int glfw_key, bool state);
	//bool isKeyEnabled(int glfw_key);
	//// Used to assign function to key
	//void setKeyFunction(int glfw_key, std::function<void()> func); // left for future reference
	// Used to call function assigned to key (out of scope)
	//void callKeyFunction(int glfw_key); // leaving this here for reference
	//static void testFunction();
	// TODO: Input::GetButtonHold(KEY_CODE); // returns true or false
	// TODO: Symmetric array can be implemented that hold functions 
	// specific to each index of this array.
	// Developers may insert functions with API calls
	// void InsertKeyFunction(unsigned int GLFW_KEY_VALUE, (void*)functionToCallWhenKeyPressed());
	
	/// Callback functions
	void _mouse_callback(double xpos, double ypos);
	void _scroll_callback(double xoffset, double yoffset);
	void _mouse_button_callback(int button, int action, int mods);

	// TODO: Maybe return a container with values for each key pressed?

};



//void Input::setKeyFunction(int glfw_key, std::function<void()> func)
//{
//	keys[glfw_key].function = func;
//}

//void Input::callKeyFunction(int glfw_key)
//{
//	keys[glfw_key].function();
//}

//int Input::count = 0;
//void Input::testFunction()
//{
//	std::cout << "testFunction" << std::endl;
//	std::cout << "count == " << count++ << std::endl;
//
//}

//void Input::keyEnabled(int glfw_key, bool state)
//{
//	keys[glfw_key].isEnabled = state;
//}

//bool Input::isKeyEnabled(int glfw_key)
//{
//	return keys[glfw_key].isEnabled;
//}