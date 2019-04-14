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