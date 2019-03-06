#include "SceneManager.h"
#include <iostream>
#include <typeinfo>
#include "CameraComponent.h"
#include "RenderManager.h"
#include "ApplicationManager.h"
#include "Serialization.h"
#include <string>

static const char* DEFAULT_SCENE_FILE_PATH = "../Assets/Scenes/";

SceneManager::SceneManager() {}

SceneManager* SceneManager::CreateManager()
{
	SceneManager* instance = &SceneManager::getInstance();

	instance->Init();

	return instance;
}

void SceneManager::Init()
{
}

Scene_ptr SceneManager::GetActiveScene()
{
	return activeScene;
}

void SceneManager::SetActiveScene(Scene_ptr scene)
{
	// ToDo: Make sure its not the same scene.
	if (activeScene != nullptr)
	{
		UnloadActiveScene();
	}
	std::cout << "Activating Scene: " << scene->name << std::endl;
	activeScene = scene;
	scene->Load();

	if (ApplicationManager::getInstance().IsEditMode() == false)
	{
		std::cout << "Looking for Camera." << std::endl;
		RenderManager::getInstance().FindCameraInScene(activeScene.get());
	}
}

void SceneManager::StartActiveScene()
{
	if (activeScene != nullptr && activeScene->isLoaded && !activeScene->isStarted)
	{
		activeScene->Start();
	}
}

void SceneManager::UpdateActiveScene()
{
	if (activeScene != nullptr && activeScene->isLoaded)
	{
		if (activeScene->isStarted)
		{
			activeScene->Update();
		}
		else
		{
			//Note: This might change later...?
			StartActiveScene();
		}
	}
}

Scene_ptr SceneManager::CreateNewScene()
{
	return CreateNewScene("New_Scene");
}

Scene_ptr SceneManager::CreateNewScene(const char * sceneName)
{
	Scene_ptr scene(new Scene(sceneName));
	scene->name = sceneName;
	SaveSceneToFile(*scene);
	std::string filename = DEFAULT_SCENE_FILE_PATH;
	filename += scene->name + ".scene";
	scene->filePath = filename;
	return scene;
}

void SceneManager::SaveActiveScene()
{
	if (activeScene != NULL)
	{
		SaveSceneToFile(*activeScene);
	}
}

void SceneManager::SaveSceneToFile(const Scene &s) {
	std::string filename = DEFAULT_SCENE_FILE_PATH;
	filename += s.name + ".scene";
	SaveSceneToFile(s, filename.c_str());
}
void SceneManager::SaveSceneToFile(const Scene &s, const char * fileName) 
{
	std::cout << "Saving Scene: " << fileName << std::endl;
	// make an archive
	std::ofstream ofs(fileName);
	if (!ofs)
	{
		std::cout << "Cannot open outfile" << std::endl;
		return;
	}
	if (ofs.bad())
	{
		std::cout << "Out File Stream BAD" << std::endl;
		return;
	}
	//std::string nS(fileName);
	//.filePath = nS;// new std::string(fileName);

	//boost::archive::text_oarchive oa(ofs);
	boost::archive::xml_oarchive oa(ofs);
	oa << BOOST_SERIALIZATION_NVP(s);

}

bool SceneManager::LoadSceneFromFileByName(Scene &s, const char * sceneName)
{
	std::string filename("../Assets/Scenes/");
	filename += std::string(sceneName) + ".scene";
	return LoadSceneFromFile(s, filename.c_str());
}

bool SceneManager::LoadSceneFromFile(Scene &s, const char * fileName)
{
	// open the archive 
	std::ifstream ifs(fileName);
	if (!ifs.good()) //Doesn't exist 
	{
		return false;
	}

	//boost::archive::text_iarchive ia(ifs);
	boost::archive::xml_iarchive ia(ifs);
	
	// restore from the archive
	ia >> BOOST_SERIALIZATION_NVP(s);

	s.filePath = fileName;
	return true;
}

void SceneManager::UnloadActiveScene()
{
	if (activeScene != nullptr && activeScene->isLoaded)
	{
		//std::cout << "Unloading Scene: " << activeScene->name << std::endl;
		activeScene->Unload();
		//activeScene = nullptr;
		activeScene.reset();

		RenderManager::getInstance().currentRenderables.clear();
	}
}

void SceneManager::ReloadSceneFromFile()
{
	if (activeScene != nullptr && activeScene->isLoaded)
	{
		//std::cout << "Reloading Scene: " << activeScene->name << std::endl;
		Scene_ptr scene(new Scene(activeScene->name.c_str()));
		std::string filePath = activeScene->filePath;
		UnloadActiveScene();
		//activeScene->PrintScene();
		//std::cout << "Reloading Scene from file" << std::endl;
		std::cout << "Reloading From File: " << filePath << std::endl;
		LoadSceneFromFile(*scene, filePath.c_str());
		std::cout << "\tFinished Reloading!" << std::endl;
		SceneManager::getInstance().SetActiveScene(scene);
		//std::cout << "Set Active!" << std::endl;
	}
}
