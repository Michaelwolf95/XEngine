//==================================================
// MAIN ENTRY POINT FOR ENGINE
//==================================================

// Defining this here to prevent duplicate definitions
// when the XEngine header is used.
#ifndef STB_DEFINE
#define STB_DEFINE  
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#endif

#include "XEngine.h"

namespace XEngine
{
	EngineEvent OnEngineInit = nullptr;
	EngineEvent OnEngineUpdate = nullptr;
	EngineEvent OnEnginePreRender = nullptr;
	EngineEvent OnEnginePostRender = nullptr;
	EngineEvent OnApplicationClose = nullptr;

	bool useDefaultSceneInitialization = true;

	// Main Game Function.
	int ENGINE_MAIN()
	{
		std::cout << "===== LAUNCHING X-ENGINE =====" << std::endl;

		// Init Managers
		ApplicationManager::CreateManager();
		GameTime::CreateManager();
		Input::CreateManager();
		AssetManager::CreateManager();
		RenderManager::CreateManager();
		SceneManager::CreateManager();
		AudioManager::CreateManager();
		PhysicsManager::CreateManager();

		if (OnEngineInit != nullptr) XEngine::OnEngineInit();

		// Create & Load Scene
		if (useDefaultSceneInitialization)
		{
			// TODO: Configure Build Config to load first scene based on a file.
			SceneManager::getInstance().LoadAndActivateSceneFromFile("../Assets/Scenes/Physics_Test_2.scene");
		}

		// FRAME LOOP
		while (!ApplicationManager::getInstance().CheckIfAppShouldClose())
		{
			ApplicationManager::getInstance().ApplicationStartUpdate();
			GameTime::getInstance().UpdateTime();
			Input::getInstance().UpdateInput();

			// MAIN UPDATE
			if (OnEngineUpdate != nullptr) OnEngineUpdate();
			SceneManager::getInstance().UpdateActiveScene();
			AudioManager::getInstance().UpdateAudio();

			PhysicsManager::getInstance().PhysicsUpdate();

			if (OnEnginePreRender != nullptr) OnEnginePreRender();

			RenderManager::getInstance().Render();

			if (OnEnginePostRender != nullptr) OnEnginePostRender();

			Input::getInstance().EndUpdateFrame();
			ApplicationManager::getInstance().ApplicationEndUpdate();
		}

		if (OnApplicationClose != nullptr) OnApplicationClose();

		ApplicationManager::getInstance().CloseApplication();
		return 0;
	}
}

// ENTRY POINT
// Used when the application is run as an executable.
//int main()
//{
//	XEngine::ENGINE_MAIN();
//}