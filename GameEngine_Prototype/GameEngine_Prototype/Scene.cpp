#include "Scene.h"
#include <iostream>

Scene::Scene()
{
	name = "New Scene";
}
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

Scene::Scene(std::vector<GameObject_ptr> gameObjects)
{
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		rootGameObjects.push_back(gameObjects[i]);
	}
}

Scene::~Scene() 
{
	std::cout << "Deconstructing Scene " << name << std::endl;
}


std::ostream & operator<<(std::ostream &os, const Scene &scene)
{
	// note: we're displaying the pointer to permit verification
	// that duplicated pointers are properly restored.
	std::vector<std::shared_ptr<GameObject>>::const_iterator it;
	for (it = scene.rootGameObjects.begin(); it != scene.rootGameObjects.end(); it++) {
		os << '\n' << std::hex << "0x" << *it << std::dec << ' ' << **it;
	}

	return os;
}


void Scene::PrintScene()
{
	std::cout << "Printing Scene '" << name << "':\nGameObjects: " << rootGameObjects.size() << std::endl;
	for (size_t i = 0; i < rootGameObjects.size(); i++)
	{
		std::cout << "   [" << i << "]: " << (rootGameObjects[i]->name) << std::endl;
		if (rootGameObjects[i]->components.size() > 0)
		{
			for (std::shared_ptr<Component> c : rootGameObjects[i]->components)
			{
				std::cout << "\t" << ((typeid(*c)).name()) << std::endl;
				//std::cout << "\tOwner:" << c->gameObject->name << std::endl;
			}
		}
	}
}

void Scene::Load()
{
	std::cout << "Loading Scene" << name << std::endl;
	PrintScene();
	isLoaded = true;
}

void Scene::Reset()
{
	Unload();
}

void Scene::Unload()
{
	std::cout << "Unloading Scene " << name << std::endl;
	std::cout << "Clearing GameObjects."<< std::endl;
	rootGameObjects.clear();
	//std::cout << "Done Clearing GameObjects." << std::endl;
	PrintScene();
	
	isLoaded = false;
	isStarted = false;
}

void Scene::Start()
{
	//std::cout << "Starting Scene..." << std::endl;
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
	//PrintScene();
	//std::cout << "Updating Scene... " << rootGameObjects.size() << std::endl;
	for (size_t i = 0; i < rootGameObjects.size(); i++)
	{
		rootGameObjects[i]->UpdateComponents();

		// ToDo: Recursively go through hierarchy. - CURRENTLY NO CHILDREN SUPPORT
	}
}

GameObject_ptr Scene::CreateGameObject(const char * name, Transform * parent)
{
	GameObject_ptr go = std::make_shared<GameObject>(GameObject(name));
	if (parent == nullptr)
	{
		rootGameObjects.push_back(go);
	}
	return go;
}

void Scene::DeleteGameObject(GameObject_ptr go)
{
	std::cout << "Deleting GameObject..." << std::endl;
	// If vector contains it, remove it.
	auto n = std::find(rootGameObjects.begin(), rootGameObjects.end(), go);
	if (n != rootGameObjects.end())
	{
		// swap the one to be removed with the last element
		// and remove the item at the end of the container
		// to prevent moving all items after '5' by one
		std::swap(*n, rootGameObjects.back());
		rootGameObjects.pop_back();

		// TODO: Delete all children.
		// TODO: Remove from parents child list.
	}
}
