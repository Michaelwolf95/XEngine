#include "Scene.h"
#include <iostream>

Scene::Scene(const char* _name)
{
	if (_name == nullptr)
	{
		name = "New Scene";
	}
	else
	{
		name = _name;
	}
}

Scene::Scene(std::vector<GameObject*> gameObjects)
{
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		rootGameObjects.push_back(gameObjects[i]);
	}
}

Scene::~Scene()
{
}

void Scene::Load()
{
	std::cout << "Loaded Scene '" << name << "'.\nGameObjects:" << std::endl;
	for (size_t i = 0; i < rootGameObjects.size(); i++)
	{
		std::cout << "   [" << i << "]: " << (rootGameObjects[i]->name) << std::endl;
		if (rootGameObjects[i]->components.size() > 0)
		{
			for (Component* c : rootGameObjects[i]->components)
			{
				std::cout << "\t" << ((typeid(*c)).name()) << std::endl;
			}
		}
	}
	isLoaded = true;
}

void Scene::Unload()
{

	isLoaded = false;
}

void Scene::Start()
{
	isStarted = true;
	for (size_t i = 0; i < rootGameObjects.size(); i++)
	{
		if (rootGameObjects[i]->isActive)
		{
			rootGameObjects[i]->StartComponents();
		}
		// ToDo: Recursively go through hierarchy. - CURRENTLY NO CHILDREN SUPPORT

	}
}

void Scene::Update()
{
	for (size_t i = 0; i < rootGameObjects.size(); i++)
	{
		rootGameObjects[i]->UpdateComponents();

		// ToDo: Recursively go through hierarchy. - CURRENTLY NO CHILDREN SUPPORT
	}
}

GameObject* Scene::CreateGameObject(const char * name, Transform * parent)
{
	GameObject* go = new GameObject(name);
	if (parent == nullptr)
	{
		rootGameObjects.push_back(go);
	}
	return go;
}
