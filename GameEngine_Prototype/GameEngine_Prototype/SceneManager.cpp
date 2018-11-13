#include "SceneManager.h"
#include <iostream>

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



