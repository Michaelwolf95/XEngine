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
#endif

namespace XEngine
{
	typedef void (*EngineEvent)(void);
	extern EngineEvent OnEngineInit;
	extern EngineEvent OnEngineUpdate;
	extern EngineEvent OnEnginePreRender;
	extern EngineEvent OnEnginePostRender;
	extern EngineEvent OnApplicationClose;

	// Flag to load scenes normally. (Used by editor.)
	// Set to false if you are replacing the scene loading functionality.
	extern bool useDefaultSceneInitialization;

	int main();

	// Used for .DLL implementation.
	extern "C"
	{
		LIBRARY_API int Engine_Main();
	}
}
