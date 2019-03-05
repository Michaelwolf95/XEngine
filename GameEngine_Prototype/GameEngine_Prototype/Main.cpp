#define GLM_FORCE_RADIANS
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_DEFINE  
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "DebugUtility.h" // Define only once
#include "Serialization.h"
#include "AssetManager.h"
#include "ApplicationManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "Time.h"
#include "Input.h"
#include "AudioManager.h"
#include "SceneEditor.h"

#include "TestScenes.h"

// ENTRY POINT
int main()
{
	std::cout << "===== LAUNCHING X-ENGINE =====" << std::endl;
	// Init Managers
	ApplicationManager::CreateManager();
	Time::CreateManager();
	Input::CreateManager();
	RenderManager::CreateManager();
	AudioManager::CreateManager();
	SceneManager::CreateManager();
	SceneEditor::CreateManager();

	// Create & Load Scene
	RunTestScene();
	//SceneEditor::getInstance().StartEditMode();
	


	// FRAME LOOP
	while (!ApplicationManager::getInstance().CheckIfAppShouldClose())
	{
		ApplicationManager::getInstance().ApplicationStartUpdate();
		Time::getInstance().UpdateTime();
		Input::getInstance().UpdateInput();

		// Editor Update
		SceneEditor::getInstance().UpdateEditor();

		// Do Game Logic here
		SceneManager::getInstance().UpdateActiveScene();

		RenderManager::getInstance().Render();

		AudioManager::getInstance().UpdateAudio();
		Input::getInstance().EndUpdateFrame();
		ApplicationManager::getInstance().ApplicationEndUpdate();
	}

	ApplicationManager::getInstance().CloseApplication();
	return 0;
}