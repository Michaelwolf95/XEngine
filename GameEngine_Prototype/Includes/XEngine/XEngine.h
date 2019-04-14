#pragma once

#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

// Used for .DLL implementation.
#include "LibraryExport.h"

//ImGui is used by the editor,
// but can be used in the engine for easy declarations.
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_inspector_extensions.h"
#include "imgui_stdlib.h"
//

#define GLM_FORCE_RADIANS
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "DebugUtility.h" // Define only once
#include "Serialization.h"
#include "ApplicationManager.h"
#include "GameTime.h"
#include "Input.h"
#include "AssetManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "PhysicsManager.h"
#include "AudioManager.h"

#include "CoreComponents.h"

namespace XEngine
{
	typedef void (*EngineEvent)(void);
	extern ENGINE_API EngineEvent OnEngineInit;
	extern ENGINE_API EngineEvent OnEngineUpdate;
	extern ENGINE_API EngineEvent OnEnginePreRender;
	extern ENGINE_API EngineEvent OnEnginePostRender;
	extern ENGINE_API EngineEvent OnApplicationClose;

	// Flag to load scenes normally. (Used by editor.)
	// Set to false if you are replacing the scene loading functionality.
	extern ENGINE_API bool useDefaultSceneInitialization;

	int ENGINE_API ENGINE_MAIN();
}
