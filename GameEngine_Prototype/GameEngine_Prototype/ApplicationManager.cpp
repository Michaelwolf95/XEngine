#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ApplicationManager.h"
/* The ApplicationManager is responsible for:
- Managing system events
- Managing GLFW and Glad systems
- Saving/Loading App Config files.

*/

ApplicationManager* ApplicationManager::instance = nullptr;
AppConfig* ApplicationManager::config = nullptr;
GLFWwindow* ApplicationManager::APP_WINDOW;

ApplicationManager::ApplicationManager()
{
	// Constructor
}
ApplicationManager::~ApplicationManager()
{
	// Deconstructor
}
// Create static instance
// ToDo: Setup the singleton manager pattern as a base class.
ApplicationManager* ApplicationManager::CreateManager()
{
	if (instance != nullptr)
	{
		printf("DUPLICATE SINGLETON DETECTED");
		return NULL;
	}
	instance = new ApplicationManager();

	//ToDo: Load config from file (Window size, etc)
	config = new AppConfig();

	instance->Init();

	if (APP_WINDOW == NULL)
	{
		std::cout << "App Window is Null! (1)" << std::endl;
	}

	return instance;
}

// Init instance and setup GLFW, etc.
int ApplicationManager::Init()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	ApplicationManager::instance->CreateAppWindow();
	if (APP_WINDOW == NULL)
	{
		std::cout << "App Window is Null!" << std::endl;
		return -1;
	}

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Set input mode
	//glfwSetInputMode(APP_WINDOW, GLFW_STICKY_KEYS, 2);


	isInitialized = true;
	return 0;
}

bool ApplicationManager::CheckIfAppShouldClose()
{
	return glfwWindowShouldClose(APP_WINDOW);
}


void ApplicationManager::ApplicationStartUpdate()
{
	// input
	// -----
	processInput(APP_WINDOW);
}

void ApplicationManager::ApplicationEndUpdate()
{
	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	glfwSwapBuffers(APP_WINDOW);
	glfwPollEvents();
}

void ApplicationManager::CloseApplication()
{
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
}

GLFWwindow* ApplicationManager::CreateAppWindow()
{
	// glfw window creation
	// --------------------
	APP_WINDOW = glfwCreateWindow(
		ApplicationManager::config->screenWidth,
		ApplicationManager::config->screenHeight,
		ApplicationManager::config->appTitle,
		NULL, NULL);
	if (APP_WINDOW == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(APP_WINDOW);
	glfwSetFramebufferSizeCallback(APP_WINDOW, framebuffer_size_callback);
	return APP_WINDOW;
}



// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}