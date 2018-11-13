#pragma once
#include <vector>
#include "GameObject.h"
class Scene
{
public:
	const char* name;
	std::vector<GameObject*> rootGameObjects;
	bool isStarted = false;
	bool isLoaded = false;
	Scene(const char* _name);
	Scene(std::vector<GameObject*> gameObjects);
	~Scene();
	void Load();
	void Unload();
	void Start();
	void Update();
	GameObject* CreateGameObject(const char* name, Transform* parent = nullptr);

	// Load 

	// Unload
};

