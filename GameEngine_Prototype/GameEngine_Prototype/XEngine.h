#pragma once
//define X_EDIT_MODE // DEFINED IN PROJECT SETTINGS
#include "LibraryExport.h"

#define GLM_FORCE_RADIANS

//ifdef X_EDIT_MODE
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_inspector_extensions.h"
#include "imgui_stdlib.h"
//endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
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
//#include "TestScenes.h"
#endif


namespace XEngine
{
	typedef void (*EngineEvent)(void);
	static EngineEvent OnEngineInit = nullptr;
	static EngineEvent OnEngineUpdate = nullptr;
	static EngineEvent OnEnginePreRender = nullptr;
	static EngineEvent OnEnginePostRender = nullptr;
	static EngineEvent OnApplicationClose = nullptr;

	int main();

	extern "C"
	{
		LIBRARY_API int Engine_Main();
	}
}
