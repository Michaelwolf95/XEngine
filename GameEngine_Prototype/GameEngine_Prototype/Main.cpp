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
#include "AudioManager.h"

#include "TestScenes.h"


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
	AudioManager::CreateManager(); //creating the only one Implementation for all audio to use

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
		AudioManager::getInstance().UpdateAudio(); //if audio does need to be updated

		ApplicationManager::getInstance().ApplicationEndUpdate();
	}

	ApplicationManager::getInstance().CloseApplication();
	return 0;
}
