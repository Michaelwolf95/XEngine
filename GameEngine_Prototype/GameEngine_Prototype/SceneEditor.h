#pragma once
//#include "Component.h"
#include "Serialization.h"
#include "Singleton.h"
#include "Scene.h"
#include "Camera.h"
#include <nlohmann/json.hpp>
#include "EditorCamera.h"

static const char* EDITOR_CONFIG_FILE_PATH = "../Settings/Editor/EditorConfig.json";

struct EditorConfig
{
	std::string firstSceneFilepath = "";
};

enum ManipToolMode
{
	None =	0,
	Move =	1,
	Rotate= 2,
	Scale =	3
};
class SceneEditor : public Singleton<SceneEditor> //: public Component
{
	friend class Singleton<SceneEditor>;
public:
	bool isInitialized = false;
	EditorConfig* editorConfig;

	GameObject* editorCameraGameObject;
	EditorCamera* editorCamera;

	static SceneEditor* CreateManager();
	int Init();
	SceneEditor();
	~SceneEditor();
	void ShutDown();

	void EditorPreRender();
	void EditorPostRender();

	void LoadEditorConfig();
	void SaveEditorConfig();
	//std::string Get
	void LoadInitialEditorScene();

	void StartEditMode();
	void ExitEditMode();
	//void StartPlayMode();
	void UpdateEditor();
	void DrawEditorGizmos();
	GameObject_ptr selectedGameObject = nullptr;
	ManipToolMode manipTool = ManipToolMode::None;
	void SelectManipTool();
	void ManipToolUpdate();
	void MoveTool();
	void RotateTool();
	void ScaleTool();
	void AddComponentMenu();
	void LoadSceneMenu();

	//imGUI update.
	void UpdateDockSpace(bool* p_open);
	void InspectorUpdate();
	void ConfigureWindowLayout();
};

