#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Singleton.h"

/* The ApplicationManager is responsible for:
- Managing system events
- Managing GLFW and Glad systems
- Saving/Loading App Config files.

*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// Application Settings - window title, screen dimensions, etc.
struct AppConfig
{
	char* appTitle = "CECS 491 Game Engine";
	unsigned int screenWidth = 800;
	unsigned int screenHeight = 600;
};

class ApplicationManager : public Singleton<ApplicationManager>
{
	friend class Singleton<ApplicationManager>;
public:
	static AppConfig* config;
	static GLFWwindow* APP_WINDOW;

	bool isInitialized = false;

	// Create static instance and configure manager
	static ApplicationManager* CreateManager();

	// Init instance and setup GLFW, etc.
	int Init();

	bool CheckIfAppShouldClose();

	void ApplicationStartUpdate();

	void ApplicationEndUpdate();

	void CloseApplication();

protected:
	ApplicationManager();
private:
	GLFWwindow* CreateAppWindow();
};