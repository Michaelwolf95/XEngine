#pragma once
//#include "Component.h"
#include "Serialization.h"
#include "Singleton.h"
#include "Scene.h"
#include "Camera.h"

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
	static SceneEditor* CreateManager();
	std::shared_ptr<Camera> editorCamera;
	//GameObject editorCameraGameObject;
	int Init();
	SceneEditor();
	~SceneEditor();
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
//private:
//	friend class boost::serialization::access;
//	//friend std::ostream & operator<<(std::ostream &os, const Component &comp);
//	template<class Archive>
//	void serialize(Archive &ar, const unsigned int version)
//	{
//		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
//	}
};

