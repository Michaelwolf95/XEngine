#include "SceneManager.h"
#include <iostream>
#include <typeinfo>
#include "AssetManager.h"
#include "CameraComponent.h"
#include "RenderManager.h"
#include "ApplicationManager.h"
#include "Serialization.h"
#include <string>

#define DEFAULT_SCENE_FILE_PATH					\
(ASSET_FILE_PATH + std::string("Scenes/"))		\
/**/
//static const char* DEFAULT_SCENE_FILE_PATH = "../Assets/Scenes/";
static const char* SCENE_FILE_EXT = ".scene";

SceneManager::SceneManager() {}

SceneManager* SceneManager::CreateManager()
{
	SceneManager* instance = &SceneManager::getInstance();

	instance->Init();

	return instance;
}

void SceneManager::Init() {}

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
void SceneManager::FixedUpdateActiveScene()
{
	if (activeScene != nullptr && activeScene->isLoaded)
	{
		if (activeScene->isStarted)
		{
			activeScene->FixedUpdate();
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
	filename += scene->name + SCENE_FILE_EXT;
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

void SceneManager::SaveSceneToFile(const Scene &s) 
{
	std::string filename = DEFAULT_SCENE_FILE_PATH;
	filename += s.name + SCENE_FILE_EXT;
	SaveSceneToFile(s, filename.c_str());
}
void SceneManager::SaveSceneToFile(const Scene &s, const char * fileName) 
{
	std::cout << "Saving Scene: " << fileName << std::endl;

	if (CreateDirectory(DEFAULT_SCENE_FILE_PATH.c_str(), NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{
		// CopyFile(...)
	}

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

	boost::archive::xml_oarchive oa(ofs);
	oa << BOOST_SERIALIZATION_NVP(s);
}

bool SceneManager::LoadSceneFromFileByName(Scene &s, const char * sceneName)
{
	std::string filename(DEFAULT_SCENE_FILE_PATH);
	filename += std::string(sceneName) + SCENE_FILE_EXT;
	return LoadSceneFromFile(s, filename.c_str());
}

bool SceneManager::LoadSceneFromFile(Scene &s, const char * fileName)
{
	std::cout << "SceneManager::LoadSceneFromFile with arguments\n";
	std::cout << "\tfileName: " << fileName << std::endl;
	// Open the archive 
	std::ifstream ifs(fileName);
	if (!ifs.good()) //Doesn't exist 
	{
		return false;
	}

	boost::archive::xml_iarchive ia(ifs);
	try
	{
		ia >> BOOST_SERIALIZATION_NVP(s);		// Restore from the archive
		s.filePath = fileName;
		std::cout << "\tscene's filePath: " << s.filePath << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "==================================================" << std::endl;
		std::cout << "ERROR: Failed to Load Scene from File.\nProbably due to outdated serialization." << std::endl;
		std::cout << e.what() << std::endl;
		std::cout << "==================================================" << std::endl;
		return false;
	}
	return true;
}

bool SceneManager::LoadAndActivateSceneFromFile(const char* fileName)
{
	bool exists = false;
	Scene_ptr scene(new Scene("TEMP_NAME"));
	if ((unsigned int)strlen(fileName) > 0)
	{
		exists = SceneManager::getInstance().LoadSceneFromFile(*scene, fileName);
		if (exists)
		{
			SceneManager::getInstance().SetActiveScene(scene);
		}
		return exists;
	}
	return false;
}

void SceneManager::UnloadActiveScene()
{
	if (activeScene != nullptr && activeScene->isLoaded)
	{
		activeScene->Unload();
		activeScene.reset();
		//RenderManager::getInstance().currentRenderables.clear();
		std::cout << "UNLOADED SCENE ================================" << std::endl;
		std::cout << "Renderables count: " << RenderManager::getInstance().currentRenderables.size() << std::endl;
	}
}

void SceneManager::ReloadSceneFromFile()
{
	if (activeScene != nullptr && activeScene->isLoaded)
	{
		Scene_ptr scene(new Scene(activeScene->name.c_str()));
		std::string filePath = activeScene->filePath;
		UnloadActiveScene();
		std::cout << "Reloading Scene '" << filePath  << "' From File..."<< std::endl;
		LoadSceneFromFile(*scene, filePath.c_str());
		std::cout << "\tFinished Reloading Scene" << std::endl;
		SceneManager::getInstance().SetActiveScene(scene);
	}
}
