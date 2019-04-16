//==================================================
// ENTRY POINT FOR EDITOR
//==================================================

#ifndef STB_DEFINE
#define STB_DEFINE  
#define STB_IMAGE_IMPLEMENTATION
// Disabled to prevent duplicate from the engine.
//include <stb/stb_image.h> 
#endif

#include "XEngineEditor.h"

#include "SceneEditor.h"
#include "ProjectSettingsManager.h"
#include "ProjectCompiler.h"

#include <Windows.h>
#include <stdio.h>
#include <string>

namespace XEngine::Editor
{
	// Engine Callbacks
	static void Editor_Init();
	static void Editor_Update();
	static void Editor_PreRender();
	static void Editor_PostRender();
	static void Editor_ApplicationClose();

	// Editor entry point function.
	int EDITOR_MAIN()
	{
		std::cout << "===== LAUNCHING X-ENGINE EDITOR =====" << std::endl;
		std::cout << "API Mode: " << API_MODE << std::endl;

		XEngine::OnEngineInit = &Editor_Init;
		XEngine::OnEngineUpdate = &Editor_Update;
		XEngine::OnEnginePreRender = &Editor_PreRender;
		XEngine::OnEnginePostRender = &Editor_PostRender;
		XEngine::OnApplicationClose = &Editor_ApplicationClose;

		XEngine::ENGINE_INITIALIZE();

		ProjectSettingsManager* settings = ProjectSettingsManager::CreateManager();
		settings->LoadProjectSettings();

		// TODO: Get project path as input, or load from file.
		//AssetManager::getInstance().setProjectFilePath(std::string("D:/VisualStudio/CECS_491_Prototype/DemoProject/"));
		AssetManager::getInstance().setProjectFilePath(settings->getProjectDirectory());
		

		//TODO: Set compiler params based on Project path.
		ProjectCompiler* compiler = ProjectCompiler::CreateManager();
		compiler->LoadProjectLibrary();

		// False for editor mode.
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
int main(int argc, char *argv[])
{
	//TODO: Set project path based on launch params.

	return XEngine::Editor::EDITOR_MAIN();
}