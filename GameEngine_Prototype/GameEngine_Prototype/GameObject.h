#pragma once
#include <vector>
#include "Component.h"
#include "Transform.h"

class GameObject
{
public:
	Transform* transform;
	bool isActive = true;
	std::string name;
	std::vector<Component*> components;
	GameObject(const char* _name = nullptr);
	~GameObject();
	void AddComponent(Component* comp);
	void RemoveComponent(Component* comp);
};

