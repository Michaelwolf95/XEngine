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

#define X_EDIT_MODE

#ifdef X_EDIT_MODE
#include "SceneEditor.h"
#else
#include "TestScenes.h"
#endif

// ENTRY POINT
int main()
{
	std::cout << "===== LAUNCHING X-ENGINE =====" << std::endl;
	// Init Managers
	ApplicationManager::CreateManager();
	Time::CreateManager();
	Input::CreateManager();
	RenderManager::CreateManager();
	SceneManager::CreateManager();
	AssetManager::CreateManager();

#ifdef X_EDIT_MODE
	SceneEditor::CreateManager();
#endif

	// Create & Load Scene
#ifdef X_EDIT_MODE
	SceneEditor::getInstance().StartEditMode();
#else
	RunTestScene();
#endif
	
	// FRAME LOOP
	while (!ApplicationManager::getInstance().CheckIfAppShouldClose())
	{
		ApplicationManager::getInstance().ApplicationStartUpdate();
		Time::getInstance().UpdateTime();
		Input::getInstance().UpdateInput();

		// Editor Update
#ifdef X_EDIT_MODE
		SceneEditor::getInstance().UpdateEditor();
#endif

		// Do Game Logic here
		SceneManager::getInstance().UpdateActiveScene();

		RenderManager::getInstance().Render();

		Input::getInstance().EndUpdateFrame();
		ApplicationManager::getInstance().ApplicationEndUpdate();
	}

	ApplicationManager::getInstance().CloseApplication();
	return 0;
}