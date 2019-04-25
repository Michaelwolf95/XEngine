#include "Scene.h"
#include <iostream>
#include <vector>
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
	std::cout << "\tDeconstructing Scene " << name << std::endl;
}


void Scene::PrintGameObject(GameObject* go, std::string prefix)
{
	std::cout << prefix << " : " << go->name << std::endl;
	if (go->components.size() > 0)
	{
		for (std::shared_ptr<Component> c : go->components)
		{
			std::cout << "\t" << ((typeid(*c)).name()) << std::endl;
		}
	}
	std::vector<GameObject*> children = go->GetChildren();
	//std::cout << "Child Count: " << children.size() << std::endl;
	for (size_t i = 0; i < children.size(); i++)
	{
		std::string cPrefix = prefix + "[" + std::to_string(i) + "]";
		PrintGameObject(children[i], cPrefix);
	}
}

void Scene::PrintScene()
{
	std::cout << "Printing Scene '" << name << "':\nGameObjects: " << rootGameObjects.size() << std::endl;
	
	for (size_t i = 0; i < rootGameObjects.size(); i++)
	{
		std::string prefix = "   [" + std::to_string(i) + "]";
		PrintGameObject(rootGameObjects[i].get(), prefix);
	}
}

void Scene::Load()
{
	std::cout << "\tLoading Scene" << name << std::endl;
	PrintScene();
	isLoaded = true;
}

void Scene::Reset()
{
	Unload();
}

void Scene::Unload()
{
	std::cout << "\tUnloading Scene " << name << std::endl;
	rootGameObjects.clear();
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
		StartGameObject(rootGameObjects[i]);
	}
}

void Scene::StartGameObject(GameObject_ptr go)
{
	if (go->IsActiveInHierarchy())
	{
		go->StartComponents();
		auto children = go->GetChildren();
		for (size_t i = 0; i < children.size(); i++)
		{
			StartGameObject(children[i]->GetSelfPtr());
		}
	}
}

void Scene::Update()
{
	for (size_t i = 0; i < rootGameObjects.size(); i++)
	{
		UpdateGameObject(rootGameObjects[i]);
	}
}

void Scene::UpdateGameObject(GameObject_ptr go)
{
	if (go->IsActiveInHierarchy())
	{
		go->UpdateComponents();
		auto children = go->GetChildren();
		for (size_t i = 0; i < children.size(); i++)
		{
			UpdateGameObject(children[i]->GetSelfPtr());
		}
	}
}

void Scene::FixedUpdate()
{
	for (size_t i = 0; i < rootGameObjects.size(); i++)
	{
		FixedUpdateGameObject(rootGameObjects[i]);
	}
}

void Scene::FixedUpdateGameObject(GameObject_ptr go)
{
	if (go->IsActiveInHierarchy())
	{
		go->FixedUpdateComponents();
		auto children = go->GetChildren();
		for (size_t i = 0; i < children.size(); i++)
		{
			FixedUpdateGameObject(children[i]->GetSelfPtr());
		}
	}
}

GameObject_ptr Scene::CreateGameObject(const char * name, Transform * parent)
{
	GameObject_ptr go = std::shared_ptr<GameObject>(new GameObject(name));
	if (parent == nullptr)
	{
		rootGameObjects.push_back(go);
	}
	allGameObjects.push_back(go);
	return go;
}

void Scene::DeleteGameObject(GameObject_ptr go)
{
	std::cout << "Scene- Deleting GameObject: " << go->name << std::endl;
	// If vector contains it, remove it.
	auto n = std::find(allGameObjects.begin(), allGameObjects.end(), go);
	if (n != allGameObjects.end())
	{
		// Move to back of the vector. (to pop-off later)
		allGameObjects.erase(n);
		allGameObjects.insert(allGameObjects.end(), go);

		// Unparent
		go->transform->SetParent(nullptr);

		// Delete all children - leaf nodes will be deleted first.
		std::vector<GameObject*> children = go->GetChildren();
		for (size_t i = 0; i < children.size(); i++)
		{
			DeleteGameObject(children[i]->GetSelfPtr());
		}

		// Reset shared_ptr (Deletion)
		go.reset();
		// Pop vector.
		allGameObjects.pop_back();
	}
	OnHierarchyUpdate();
}

void Scene::OnHierarchyUpdate()
{
	rootGameObjects.clear();
	// TODO: Delete Empty or null objects if they exist.

	for (size_t i = 0; i < allGameObjects.size(); i++)
	{
		if (allGameObjects[i] == nullptr)
		{
			std::cout << "NULL in GameObjects!" << std::endl;
			break;
		}
		if (allGameObjects[i]->transform->GetParent() == nullptr)
		{
			rootGameObjects.push_back(allGameObjects[i]);
		}
	}
}

void Scene::AddExistingGameObject(GameObject_ptr go)
{
	allGameObjects.push_back(go);
	if (go->transform->GetParent() == nullptr)
	{
		rootGameObjects.push_back(go);
	}
}
