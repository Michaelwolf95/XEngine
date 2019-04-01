//define X_EDIT_MODE // DEFINED IN PROJECT SETTINGS

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

	// Used for .DLL implementation.
	extern "C" {
		int Engine_Main()
		{
			return main();
		}
	}

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

		//ifdef X_EDIT_MODE
		//SceneEditor::CreateManager();
		//endif

		if (OnEngineInit != nullptr) XEngine::OnEngineInit();

		//ifdef X_EDIT_MODE
		//SceneEditor::getInstance().StartEditMode();
		//else
		//endif

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
			Time::getInstance().UpdateTime();
			Input::getInstance().UpdateInput();

			// Editor Update
			if (OnEngineUpdate != nullptr) OnEngineUpdate();
			//ifdef X_EDIT_MODE
			//SceneEditor::getInstance().UpdateEditor();
			//endif

			// Do Game Logic here
			SceneManager::getInstance().UpdateActiveScene();
			AudioManager::getInstance().UpdateAudio();

			PhysicsManager::getInstance().PhysicsUpdate();

			if (OnEnginePreRender != nullptr) OnEnginePreRender();
			//ifdef X_EDIT_MODE
			//SceneEditor::getInstance().EditorPreRender();
			//endif

			RenderManager::getInstance().Render();

			if (OnEnginePostRender != nullptr) OnEnginePostRender();
			//ifdef X_EDIT_MODE
			//SceneEditor::getInstance().EditorPostRender();
			//endif

			Input::getInstance().EndUpdateFrame();
			ApplicationManager::getInstance().ApplicationEndUpdate();
		}

		if (OnApplicationClose != nullptr) OnApplicationClose();
		//ifdef X_EDIT_MODE
		//SceneEditor::getInstance().ShutDown();
		//endif

		ApplicationManager::getInstance().CloseApplication();
		return 0;
	}
}