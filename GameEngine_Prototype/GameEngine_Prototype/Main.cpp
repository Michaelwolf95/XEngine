#define X_EDIT_MODE

#define GLM_FORCE_RADIANS

#ifdef X_EDIT_MODE
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_inspector_extensions.h"
#include "imgui_stdlib.h"
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_DEFINE  
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <iostream>
#include "DebugUtility.h" // Define only once
#include "Serialization.h"
#include "ApplicationManager.h"
#include "Time.h"
#include "Input.h"
#include "AssetManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "PhysicsManager.h"
#include "AudioManager.h"
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
	AssetManager::CreateManager();
	RenderManager::CreateManager();
	SceneManager::CreateManager();
	AudioManager::CreateManager();
	PhysicsManager::CreateManager();

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
		AudioManager::getInstance().UpdateAudio();

		PhysicsManager::getInstance().PhysicsUpdate();

		#ifdef X_EDIT_MODE
		SceneEditor::getInstance().EditorPreRender();
		#endif

		RenderManager::getInstance().Render();

		#ifdef X_EDIT_MODE
		SceneEditor::getInstance().EditorPostRender();
		#endif

		Input::getInstance().EndUpdateFrame();
		ApplicationManager::getInstance().ApplicationEndUpdate();
	}

	#ifdef X_EDIT_MODE
	SceneEditor::getInstance().ShutDown();
	#endif

	ApplicationManager::getInstance().CloseApplication();
	return 0;
}