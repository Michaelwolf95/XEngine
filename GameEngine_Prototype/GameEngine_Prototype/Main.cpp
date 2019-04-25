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
	ENGINE_API EngineEvent OnEngineInit = nullptr;
	ENGINE_API EngineEvent OnEngineUpdate = nullptr;
	ENGINE_API EngineEvent OnEnginePreRender = nullptr;
	ENGINE_API EngineEvent OnEnginePostRender = nullptr;
	ENGINE_API EngineEvent OnApplicationClose = nullptr;
	ENGINE_API bool engineInitialized = false;

	ENGINE_API bool useDefaultSceneInitialization = true;

	int ENGINE_API ENGINE_INITIALIZE()
	{
		if (engineInitialized)
			return 0;
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

		engineInitialized = true;
		return 0;
	}

	// Main Game Function.
	int ENGINE_API ENGINE_MAIN()
	{
		std::cout << "===== LAUNCHING X-ENGINE =====" << std::endl;
		std::cout << "API Mode: " << API_MODE << std::endl;
		
		ENGINE_INITIALIZE();

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

#include <Windows.h>
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	//std::cout << "DLL_MAIN" << std::endl;
	return TRUE;
}

// ENTRY POINT
// Used when the application is run as an executable.
//int main()
//{
//	XEngine::ENGINE_MAIN();
//}