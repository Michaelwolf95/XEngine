//==================================================
// ENTRY POINT FOR EDITOR
//==================================================

#ifndef STB_DEFINE
#define STB_DEFINE  
#define STB_IMAGE_IMPLEMENTATION
// Disabled to prevent duplicate from the engine.
//include <stb/stb_image.h> 
#endif

#include "XEngine.h"

#include "SceneEditor.h"

namespace XEngine::Editor
{
	// Engine Callbacks
	static void Editor_Init();
	static void Editor_Update();
	static void Editor_PreRender();
	static void Editor_PostRender();
	static void Editor_ApplicationClose();

	typedef void(*f_funci)();

	// Editor entry point function.
	int EDITOR_MAIN()
	{
		std::cout << "===== LAUNCHING X-ENGINE EDITOR =====" << std::endl;

		HINSTANCE hGetProcIDDLL = LoadLibrary(TEXT("DemoProject.dll"));
		if (hGetProcIDDLL == NULL)
		{
			std::cout << "[DLL]: ERROR -  Could not load the dynamic library." << std::endl;
			//return EXIT_FAILURE;
		}
		else
		{
			std::cout << "[DLL]: Loaded DLL Library!" << std::endl;
		}
		// resolve function address here
		f_funci funci = (f_funci)GetProcAddress(hGetProcIDDLL, "print_some_number");
		if (funci == NULL)
		{
			std::cout << "[ERROR]: Could not locate the function" << std::endl;
			//return EXIT_FAILURE;
		}
		else
		{
			std::cout << "Running Function:" << std::endl;
			funci();
		}

		XEngine::OnEngineInit = &Editor_Init;
		XEngine::OnEngineUpdate = &Editor_Update;
		XEngine::OnEnginePreRender = &Editor_PreRender;
		XEngine::OnEnginePostRender = &Editor_PostRender;
		XEngine::OnApplicationClose = &Editor_ApplicationClose;

		XEngine::useDefaultSceneInitialization = false;

		return XEngine::ENGINE_MAIN();
	}

	void Editor_Init()
	{
		std::cout << "Initializing Editor. =======================================" << std::endl;
		XEngine::Editor::SceneEditor::CreateManager();

		XEngine::Editor::SceneEditor::getInstance().StartEditMode();
	}
	void Editor_Update()
	{
		XEngine::Editor::SceneEditor::getInstance().UpdateEditor();

		if (Input::GetKeyDown(GLFW_KEY_SPACE))
		{
			std::cout << "Editor: " << XEngine::number << std::endl;
		}

	}
	void Editor_PreRender()
	{
		XEngine::Editor::SceneEditor::getInstance().EditorPreRender();
	}
	void Editor_PostRender()
	{
		XEngine::Editor::SceneEditor::getInstance().EditorPostRender();
	}
	void Editor_ApplicationClose()
	{
		XEngine::Editor::SceneEditor::getInstance().ShutDown();
	}
}

// ENTRY POINT
int main()
{
	return XEngine::Editor::EDITOR_MAIN();
}