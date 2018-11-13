#pragma once
#include "Scene.h"
//ToDo: Track scenes and be able to swap between them.
class SceneManager
{
public:
	static SceneManager* instance;
	static SceneManager* CreateManager();
	SceneManager();
	~SceneManager();
	void Init();
	void SetActiveScene(Scene* scene);
	void StartActiveScene();
	void UpdateActiveScene();
private:
	Scene* activeScene;
};

