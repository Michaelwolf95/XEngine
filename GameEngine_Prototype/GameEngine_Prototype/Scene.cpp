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

Scene::Scene(std::vector<gameObject_pointer> gameObjects)
{
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		rootGameObjects.push_back(gameObjects[i]);
	}
}

Scene::~Scene() {}



std::ostream & operator<<(std::ostream &os, const Scene &scene)
{
	// note: we're displaying the pointer to permit verification
	// that duplicated pointers are properly restored.
	std::vector<GameObject*>::const_iterator it;
	for (it = scene.rootGameObjects.begin(); it != scene.rootGameObjects.end(); it++) {
		os << '\n' << std::hex << "0x" << *it << std::dec << ' ' << **it;
	}

	return os;
}


void Scene::PrintScene()
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
}

void Scene::Load()
{
	PrintScene();
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
