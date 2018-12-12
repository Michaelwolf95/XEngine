#define GLM_FORCE_RADIANS
#define STB_DEFINE  
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "DebugUtility.h" // Define only once
#include "AssetManager.h"
#include "ApplicationManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "Time.h"
#include "Input.h"

#include "SampleScene.h"

// ENTRY POINT
int main()
{
	std::cout << "===== LAUNCHING X-ENGINE =====" << std::endl;
	// Init Managers
	ApplicationManager::CreateManager();
	RenderManager::CreateManager();
	SceneManager::CreateManager();
	Time::CreateManager();
	Input::CreateManager();

	// Create & Load Scene (from SampleScene.h)
	CreateSampleScene();

	// FRAME LOOP
	while (!ApplicationManager::getInstance().CheckIfAppShouldClose())
	{
		ApplicationManager::getInstance().ApplicationStartUpdate();
		Time::getInstance().UpdateTime();
		Input::getInstance().UpdateInput();

		// Do Game Logic here
		SceneManager::getInstance().UpdateActiveScene();

		RenderManager::getInstance().Render();

		ApplicationManager::getInstance().ApplicationEndUpdate();
	}

	ApplicationManager::getInstance().CloseApplication();
	return 0;
}
