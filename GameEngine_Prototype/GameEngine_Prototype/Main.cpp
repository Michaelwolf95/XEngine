//define X_EDIT_MODE // DEFINED IN PROJECT SETTINGS

#ifndef STB_DEFINE
#define STB_DEFINE  
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#endif

#include "XEngine.h"

namespace XEngine
{
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

		// TEST CALLBACKS
		std::cout << *XEngine::OnEngineInit << std::endl;
		XEngine::OnEngineInit();

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

		//if(OnEngineInit != nullptr) 
		//XEngine::OnEngineInit();

		// Create & Load Scene
		#ifdef X_EDIT_MODE
		SceneEditor::getInstance().StartEditMode();
		#else
		// TODO: Configure Build Config to load first scene based on a file.
		SceneManager::getInstance().LoadAndActivateSceneFromFile("../Assets/Scenes/Physics_Test_2.scene");
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
}