#pragma once
//#include "Component.h"
#include "XEngine.h"
#include "Serialization.h"
#include "Singleton.h"
#include "Scene.h"
#include "Camera.h"
#include <nlohmann/json.hpp>
#include "EditorCamera.h"
#include <filesystem>

namespace XEngine::Editor
{
	#define EDITOR_DIRECTORY					\
	(std::string("../"))						\
	/**/
	#define EDITOR_ASSETS_DIRECTORY						\
	(EDITOR_DIRECTORY + std::string("EditorAssets/"))		\
	/**/
	#define EDITOR_SETTINGS_FILE_PATH										\
	(PROJECT_FILE_PATH + std::string("Settings/Editor/"))					\
	/**/
	#define EDITOR_CONFIG_FILE_PATH											\
	(EDITOR_SETTINGS_FILE_PATH + std::string("SceneEditorConfig.json"))		\
	/**/
	//static const char* EDITOR_CONFIG_FILE_PATH = "../Settings/Editor/SceneEditorConfig.json";

	//ToDo: Replace with EditorSettingsManager.
	struct SceneEditorConfig
	{
		std::string firstSceneFilepath = "";
	};

	enum ManipToolMode
	{
		None = 0,
		Move = 1,
		Rotate = 2,
		Scale = 3
	};
	class SceneEditor : public Singleton<SceneEditor> //: public Component
	{
		friend class Singleton<SceneEditor>;
	public:
		bool isInitialized = false;
		SceneEditorConfig* editorConfig;

		GameObject* editorCameraGameObject;
		EditorCamera* editorCamera;

		GameObject_ptr selectedGameObject = nullptr;
		ManipToolMode manipTool = ManipToolMode::None;

		static SceneEditor* CreateManager();
		int Init();
		SceneEditor();
		~SceneEditor();
		void ShutDown();

		void EditorPreRender();
		void EditorPostRender();

		void LoadEditorConfig();
		void SaveEditorConfig();
		void LoadInitialEditorScene();

		void StartEditMode();
		void ExitEditMode();
		//void StartPlayMode();
		void UpdateEditor();
		void DrawEditorGizmos();

		void SelectManipTool();
		void ManipToolUpdate();
		void MoveTool();
		void RotateTool();
		void ScaleTool();
		//void AddComponentMenu();
		//void LoadSceneMenu();

		void ScreenPosToWorldRay(
			int mouseX, int mouseY,             // Mouse position, in pixels, from bottom-left corner of the window
			int screenWidth, int screenHeight,  // Window size, in pixels
			glm::mat4 ViewMatrix,               // Camera position and orientation
			glm::mat4 ProjectionMatrix,         // Camera parameters (ratio, field of view, near and far planes)
			glm::vec3& out_origin,              // Ouput : Origin of the ray. /!\ Starts at the near plane, so if you want the ray to start at the camera's position instead, ignore this.
			glm::vec3& out_direction            // Ouput : Direction, in world space, of the ray that goes "through" the mouse.
		);
		void UpdateCursorSelection();

		//ImGui update.
		void UpdateGUI();
		void UpdateDockSpace(bool* p_open);
		void InspectorUpdate();
		void HierarchyUpdate();
		void DrawGameObjectTreeNode(GameObject * go, std::string label);

		void AssetFolderMenuUpdate();
		void DrawDirectoryTreeNode(const char * directory);
		void DrawFileTreeNode(const char * directory);

		void ConfigureWindowLayout();
	};

}