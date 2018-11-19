#pragma once
#include "Singleton.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h> 
enum KeyCode
{

};

void INPUT_MOUSE_CALLBACK(GLFWwindow* window, double xpos, double ypos);
void INPUT_SCROLL_CALLBACK(GLFWwindow* window, double xoffset, double yoffset);
//TODO: Store mouse callback output every frame from 
/*
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
*/
class Input : public Singleton<Input>
{
	friend class Singleton<Input>;
public:
	double xPos;
	double yPos;
	double xDeltaPos;
	double yDeltaPos;
	double xScrollOffset;
	double yScrollOffset;
	bool firstMouse = true;
	Input();
	~Input();
	void _mouse_callback(double xpos, double ypos);
	void _scroll_callback(double xoffset, double yoffset);

	bool isInitialized = false;

	// Create static instance and configure manager
	static Input* CreateManager();

	// Init instance and setup GLFW, etc.
	void Init();
};

