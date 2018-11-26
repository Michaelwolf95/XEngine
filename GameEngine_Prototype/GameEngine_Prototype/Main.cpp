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

#include "SampleScene.h"


// ENTRY POINT
int main()
{
	std::cout << "===== LAUNCHING CECS_491 GAME ENGINE =====" << std::endl;
	// Init Managers
	ApplicationManager::CreateManager();
	RenderManager::CreateManager();
	SceneManager::CreateManager();
	Time::CreateManager();
	Input::getInstance().CreateManager();

	// Create & Load Scene (from SampleScene.h)
	CreateSampleScene();

	// FRAME LOOP
	while (!ApplicationManager::getInstance().CheckIfAppShouldClose())
	{
		ApplicationManager::getInstance().ApplicationStartUpdate();
		Time::getInstance().UpdateTime();
		Input::getInstance().UpdateInput();

		for (int i = 0; i < 350; i++)
		{
			if (Input::GetKeyDown(i) == true)
			{
				std::cout << "Key number " << i << " was preseed!" << std::endl;
				if (i == GLFW_KEY_P) Input::ToggleCursor();
			}
		}

		std::cout << Input::GetDeltaPosX() << std::endl;


		// Do Game Logic here
		SceneManager::getInstance().UpdateActiveScene();

		RenderManager::getInstance().Render();

		ApplicationManager::getInstance().ApplicationEndUpdate();
	}

	ApplicationManager::getInstance().CloseApplication();
	return 0;
}
