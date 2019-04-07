#pragma once
#include "Serialization.h"
#include "Singleton.h"
#include "Scene.h"

//std::ostream & operator<<(std::ostream &os, const Scene &scene);
//ToDo: Track scenes and be able to swap between them.
class SceneManager : public Singleton<SceneManager>
{
	friend class Singleton<SceneManager>;
public:
	static SceneManager* CreateManager();
	SceneManager();
	void Init();
	Scene_ptr GetActiveScene();
	void SetActiveScene(Scene_ptr scene);
	void StartActiveScene();
	void UpdateActiveScene();
	void FixedUpdateActiveScene();

	Scene_ptr CreateNewScene();
	Scene_ptr CreateNewScene(const char * sceneName);

	void SaveActiveScene();
	void SaveSceneToFile(const Scene &s);
	void SaveSceneToFile(const Scene &s, const char * fileName);
	bool LoadSceneFromFileByName(Scene &s, const char * sceneName);
	bool LoadSceneFromFile(Scene &s, const char * fileName);

	bool LoadAndActivateSceneFromFile(const char* fileName);

	void UnloadActiveScene();
	void ReloadSceneFromFile();
private:
	Scene_ptr activeScene;
};

