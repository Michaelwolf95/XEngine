#pragma once
#include "Singleton.h"
#include "Scene.h"
//ToDo: Track scenes and be able to swap between them.
class SceneManager : public Singleton<SceneManager>
{
	friend class Singleton<SceneManager>;
public:
	static SceneManager* CreateManager();
	SceneManager();
	void Init();
	void SetActiveScene(Scene* scene);
	void StartActiveScene();
	void UpdateActiveScene();
private:
	Scene* activeScene;
};

