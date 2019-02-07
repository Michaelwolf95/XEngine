#include "SceneManager.h"
#include <iostream>
#include <typeinfo>
#include "CameraComponent.h"
#include "RenderManager.h"
#include "Serialization.h"

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

Scene* SceneManager::GetActiveScene()
{
	return activeScene;
}

void SceneManager::SetActiveScene(Scene* scene)
{
	// ToDo: Make sure its not the same scene.
	if (activeScene != nullptr)
	{
		std::cout << "Unloading Scene: " << activeScene->name << std::endl;
		activeScene->Unload();
	}
	std::cout << "Loading Scene: " << scene->name << std::endl;
	activeScene = scene;
	scene->Load();

	// Init Camera for RenderManager
	CameraComponent* camera = nullptr;
	for (GameObject* go : activeScene->rootGameObjects)
	{
		// Finds the first object of the type CameraComponent
		// Just checks roots for now. - change to search all later.
		if (go->FindComponent(typeid(CameraComponent), (void**)&camera)) // Pointer to a pointer!
		{
			RenderManager::getInstance().setCurrentCamera(camera);
			break;
		}
	}
}

void SceneManager::StartActiveScene()
{
	if (activeScene != nullptr && !activeScene->isStarted)
	{
		activeScene->isStarted = true;
		activeScene->Start();
	}
}

void SceneManager::UpdateActiveScene()
{
	if (activeScene != nullptr)
	{
		if (activeScene->isStarted)
		{
			activeScene->Update();
		}
		else
		{
			//Note: This might change later.
			activeScene->Start();
		}
	}
}

void SceneManager::SaveSceneToFile(const Scene &s) {
	std::string filename = "../Assets/Scenes/";
	//filename += s.name + ".txt";
	filename += s.name + ".scene";
	SaveSceneToFile(s, filename.c_str());
}
void SceneManager::SaveSceneToFile(const Scene &s, const char * fileName) {

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

	// restore the schedule from the archive
	ia >> BOOST_SERIALIZATION_NVP(s);

	return true;
}
