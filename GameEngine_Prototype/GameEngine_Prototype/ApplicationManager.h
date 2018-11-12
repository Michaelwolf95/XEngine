#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

/* The ApplicationManager is responsible for:
- Managing system events
- Managing GLFW and Glad systems
- Saving/Loading App Config files.

*/

//static GLFWwindow* APP_WINDOW;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
struct AppConfig
{
	char* appTitle = "CECS 491 Game Engine";
	unsigned int screenWidth = 800;
	unsigned int screenHeight = 600;
};

class ApplicationManager
{
public:
	static ApplicationManager* instance;
	static AppConfig* config;
	static GLFWwindow* APP_WINDOW;
	bool isInitialized = false;

	ApplicationManager();
	~ApplicationManager();

	// Create static instance
	static ApplicationManager* CreateManager();

	// Init instance and setup GLFW, etc.
	int Init();

	bool CheckIfAppShouldClose();

	void ApplicationStartUpdate();

	void ApplicationEndUpdate();

	void CloseApplication();

private:
	GLFWwindow* CreateAppWindow();
};