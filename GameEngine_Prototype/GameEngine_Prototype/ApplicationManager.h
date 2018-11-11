#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

/* The ApplicationManager is responsible for:
- Managing system events
- Managing GLFW and Glad systems
- Saving/Loading App Config files.

*/

static GLFWwindow* APP_WINDOW;

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

	bool isInitialized = false;
	ApplicationManager()
	{
		// Constructor
	}
	~ApplicationManager()
	{
		// Deconstructor
	}
	// Create static instance
	// ToDo: Setup the singleton manager pattern as a base class.
	static ApplicationManager* CreateManager()
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
		return instance;
	}

	// Init instance and setup GLFW, etc.
	int Init()
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
		APP_WINDOW = ApplicationManager::instance->CreateAppWindow();
		if (APP_WINDOW == NULL)
		{
			return -1;
		}

		// glad: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}
		isInitialized = true;

		return 0;
	}

	bool CheckIfAppShouldClose()
	{
		return glfwWindowShouldClose(APP_WINDOW);
	}


	void ApplicationStartUpdate()
	{
		// input
		// -----
		processInput(APP_WINDOW);
	}

	void ApplicationEndUpdate()
	{
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(APP_WINDOW);
		glfwPollEvents();
	}

	void CloseApplication()
	{
		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwTerminate();
	}

private:
	GLFWwindow* CreateAppWindow()
	{
		// glfw window creation
		// --------------------
		GLFWwindow* window = glfwCreateWindow(
			ApplicationManager::config->screenWidth,
			ApplicationManager::config->screenHeight,
			ApplicationManager::config->appTitle,
			NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return NULL;
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		return window;
	}


};


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

