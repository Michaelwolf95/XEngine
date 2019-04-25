#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <nlohmann\json.hpp>
#include "ApplicationManager.h"
#include "AssetManager.h"
#include "SceneManager.h"

/* The ApplicationManager is responsible for:
   - Managing system events
   - Managing GLFW and Glad systems
   - Saving/Loading App Config files.

*/

#define APP_CONFIG_FILE_PATH						\
(PROJECT_FILE_PATH + std::string("Settings/"))		\
/**/

AppConfig* ApplicationManager::config = nullptr;
GLFWwindow* ApplicationManager::APP_WINDOW;

ApplicationManager::ApplicationManager() {}

// Create static instance & configure manager
ApplicationManager* ApplicationManager::CreateManager()
{
	ApplicationManager* instance = &ApplicationManager::getInstance();

	//ToDo: Load config from file (Window size, etc)
	//LoadAppConfig() happens in Init()
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
	//Load application configurations from JSON file
	LoadAppConfig();

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_SCALE_TO_MONITOR); // macOS specific?

	//for high dpi scaling
	//glfwGetFramebufferSize();
	//glfwSetFramebufferSizeCallback();

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	ApplicationManager::getInstance().CreateAppWindow();
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

	//glfwSetWindowSizeCallback(APP_WINDOW, );

	// Set input mode (This should go into the Input manager class)
	//glfwSetInputMode(APP_WINDOW, GLFW_STICKY_KEYS, 2);

	ConfigureWindowLayout();

	isInitialized = true;
	return 0;
}

bool ApplicationManager::CheckIfAppShouldClose()
{
	return glfwWindowShouldClose(APP_WINDOW);
}


void ApplicationManager::ApplicationStartUpdate()
{
	// input - Close app when escape is hit.
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
	// Saves application configuration before window is closed
	SaveAppConfig();

	// Unload current scene to safely close.
	SceneManager::getInstance().UnloadActiveScene();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
}

bool ApplicationManager::IsEditMode()
{
	return isEditMode;
}

void ApplicationManager::SetEditMode(bool mode)
{
	isEditMode = mode;
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

void ApplicationManager::LoadAppConfig()
{
	std::string appConfigPath = std::string(APP_CONFIG_FILE_PATH) + "AppConfig.json";

	try
	{
		std::ifstream file(appConfigPath); 

		file >> appSettings;

		std::string tempStr = appSettings["Window_Title"];
		tempChar = new char[tempStr.length() + 1];
		strcpy_s(tempChar, tempStr.length() + 1, tempStr.c_str());
		this->config->appTitle = tempChar;

		this->config->screenHeight = appSettings["Screen_Height"];
		this->config->screenWidth = appSettings["Screen_Width"];

		file.close();
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: Problem loading AppConfig.json." << std::endl;
	}
}

void ApplicationManager::SaveAppConfig()
{
	try
	{
		std::string appConfigPath = std::string(APP_CONFIG_FILE_PATH) + "AppConfig.json";

		std::ofstream file(appConfigPath);

		this->saveSettings["Window_Title"] = this->config->appTitle;
		this->saveSettings["Screen_Height"] = this->config->screenHeight;
		this->saveSettings["Screen_Width"] = this->config->screenWidth;

		file << saveSettings.dump(4) << std::endl;

		file.close();

		delete[] tempChar;
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: Problem saving to file." << std::endl;
	}
}

void ApplicationManager::ConfigureWindowLayout()
{
	// Set position and size of console.
	int border_thickness = GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYCAPTION);
	RECT screenDimensions; //Without Taskbar
	BOOL fResult = SystemParametersInfo(SPI_GETWORKAREA, 0, &screenDimensions, 0);
	HWND console = GetConsoleWindow();
	int consoleWidth = 600;
	int consoleHeight = screenDimensions.bottom - border_thickness;
	int consoleX = 0;
	int consoleY = 0;// border_thickness;
	MoveWindow(console, consoleX, consoleY, consoleWidth, consoleHeight, TRUE);


	// Set position of window.
	int appPosX = consoleWidth;
	int appPosY = border_thickness; // border_thickness
	glfwSetWindowPos(APP_WINDOW, appPosX, appPosY);
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