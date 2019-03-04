#pragma once
#include "Singleton.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <vector>
#include <functional>
#define m_arr_sz 10
#define k_arr_sz 350
#define k_arr_start 32 // Skips first 32 integers to save CPU time.

/*	Requirements:
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

void INPUT_MOUSE_CALLBACK(GLFWwindow* window, double xpos, double ypos);
void INPUT_SCROLL_CALLBACK(GLFWwindow* window, double xoffset, double yoffset);
void INPUT_MOUSE_BUTTON_CALLBACK(GLFWwindow* window, int button, int action, int mods);

// TODO: descriptions of class and members
class Input : public Singleton<Input>
{
private:
	struct InputState
	{		
		bool isPressed = false;
		bool wasPressed = false;
	};

	friend class Singleton<Input>;

	bool isInitialized = false;
	bool mouseIdle = true;
	bool firstMouse = true;
	bool isCursorEnabled;
	bool scrollUpdated = false;
	double xPosLast;
	double yPosLast;
	double xPos; // TODO: Discuss why we should use 64bit double over 32bit float
	double yPos;
	double xScrollOffset;
	double yScrollOffset;
	InputState mouse[m_arr_sz];
	InputState keys[k_arr_sz]; // tracks which keys are pressed
	Input();
	bool getKeyDown(int glfw_key);
	bool getKey(int glfw_key);
	bool getKeyUp(int glfw_key);
	bool getMouseButtonDown(int glfw_mouse_button);
	bool getMouseButton(int glfw_mouse_button);
	bool getMouseButtonUp(int glfw_mouse_button);
	void validateMouseInputValue(int glfw_mouse_button);
	void validateKeyInputValue(int glfw_key);
	bool getMousePosX();
	bool getMousePosY();
	double getDeltaPosX();
	double getDeltaPosY();
	inline double getDelta(double &pos, double &lastpos);
	glm::vec2 getMousePos();
	glm::vec2 getMouseDelta();
	void showCursor(bool enable);
	void checkKeyInputs();
	bool toggleCursor();

protected:
	// Init instance and setup GLFW, etc.
	void Init();

public:
	// Create static instance and configure manager
	static Input* CreateManager();
	// Update keys for each frame.
	void UpdateInput();
	void EndUpdateFrame();
	~Input();

	/// API calls below
	/// Mouse inputs
	static glm::vec2 GetMousePos(); // Returns mouse position as a vec2
	static glm::vec2 GetMouseDelta(); // TODO: Should we remove glm objects and replace with standard library objets (pair)?
	static bool GetMouseButtonDown(int glfw_mouse_button); 
	static bool GetMouseButton(int glfw_mouse_button); 
	static bool GetMouseButtonUp(int glfw_mouse_button); 
	static double GetScrollOffsetX();
	static double GetScrollOffsetY();
	static double GetMousePosX();
	static double GetMousePosY();
	static double GetDeltaPosX();
	static double GetDeltaPosY();
	static void ShowCursor(bool enable);
	static void ToggleCursor();

	/// Keyboard inputs
	static bool GetKeyDown(int glfw_key); 
	static bool GetKey(int glfw_key); 
	static bool GetKeyUp(int glfw_key);

	// TODO: Symmetric array can be implemented that hold functions 
	// specific to each index of this array.
	// Developers may insert functions with API calls
	
	/// Callback functions
	void _mouse_callback(double xpos, double ypos);
	void _scroll_callback(double xoffset, double yoffset);
	void _mouse_button_callback(int button, int action, int mods);
	
	// TODO: Maybe return a container with values for each key pressed?
};
