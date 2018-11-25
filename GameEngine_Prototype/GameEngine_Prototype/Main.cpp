#define GLM_FORCE_RADIANS
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_DEFINE  
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "DebugUtility.h" // Define only once
#include "AssetManager.h"
#include "ApplicationManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "Time.h"
#include "Input.h"

#include "TestScenes.h"

//unsigned int __stdcall mythread(void* data);
//long __stdcall WindowProcedure(HWND window, unsigned int msg, WPARAM wp, LPARAM lp);

void functionInMain();

// ENTRY POINT
int main()
{
	std::cout << "===== LAUNCHING CECS_491 GAME ENGINE =====" << std::endl;
	// Init Managers
	ApplicationManager::CreateManager();
	RenderManager::CreateManager();
	SceneManager::CreateManager();
	Time::CreateManager();
	Input::CreateManager();

	// Create & Load Scene
	RunTestScene();

	// FRAME LOOP
	while (!ApplicationManager::getInstance().CheckIfAppShouldClose())
	{
		ApplicationManager::getInstance().ApplicationStartUpdate();
		Time::getInstance().UpdateTime();
		Input::getInstance().UpdateInput();

		// Do Game Logic here
		SceneManager::getInstance().UpdateActiveScene();

		RenderManager::getInstance().Render();
		//Input::getInstance().setKeyFunction(32, functionInMain);
		//Input::getInstance().checkKeyInputs

		ApplicationManager::getInstance().ApplicationEndUpdate();
	}

	ApplicationManager::getInstance().CloseApplication();
	return 0;
}

void functionInMain()
{
	std::cout << "Printing from a function in main!" << std::endl;
}
