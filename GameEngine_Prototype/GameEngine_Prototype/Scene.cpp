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
	std::cout << "\tClearing GameObjects."<< std::endl;
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
		allGameObjects.erase(n);
		allGameObjects.insert(allGameObjects.end(), go);
		//std::move(allGameObjects.begin(), allGameObjects.end())

		// TODO: Remove from parents child list.
		go->transform->SetParent(nullptr);

		// TODO: Delete all children.
		std::vector<GameObject*> children = go->GetChildren();
		for (size_t i = 0; i < children.size(); i++)
		{
			DeleteGameObject(children[i]->GetSelfPtr());
		}

		// From Tutorial:
		//    swap the one to be removed with the last element and remove the item at the end of the container
		//    to prevent moving all items after '5' by one
		//auto n = std::find(allGameObjects.begin(), allGameObjects.end(), go);
		std::cout << "-- Deleting " << go->name << std::endl;
		//std::remov
		//std::swap(go, allGameObjects.back());
		go.reset();
		allGameObjects.pop_back();
	}
	OnHierarchyUpdate();
}

void Scene::OnHierarchyUpdate()
{
	rootGameObjects.clear();
	// TODO: Delete Empty or null
	for (size_t i = 0; i < allGameObjects.size(); i++)
	{

	}
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

//GameObject_ptr Scene::FindSharedGameObjectPointer(GameObject* go)
//{
//	for (size_t i = 0; i < allGameObjects.size(); i++)
//	{
//		if(allGameObjects[i].get() )
//	}
//	return GameObject_ptr();
//}

