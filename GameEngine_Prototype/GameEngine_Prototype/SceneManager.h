#pragma once
#include "Serialization.h"
#include "Singleton.h"
#include "Scene.h"

std::ostream & operator<<(std::ostream &os, const Scene &scene);
//ToDo: Track scenes and be able to swap between them.
class SceneManager : public Singleton<SceneManager>
{
	friend class Singleton<SceneManager>;
public:
	static SceneManager* CreateManager();
	SceneManager();
	void Init();
	Scene* GetActiveScene();
	void SetActiveScene(Scene* scene);
	void StartActiveScene();
	void UpdateActiveScene();

	void SaveActiveScene();
	void SaveSceneToFile(const Scene &s);
	void SaveSceneToFile(const Scene &s, const char * fileName);
	bool LoadSceneFromFileByName(Scene &s, const char * sceneName);
	bool LoadSceneFromFile(Scene &s, const char * fileName);

	void ReloadSceneFromFile();
private:
	Scene* activeScene;
};

