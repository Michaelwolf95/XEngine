#pragma once
#include <vector>
//#include <function>
#include "Component.h"
#include "Transform.h"

class GameObject
{
public:
	Transform* transform;
	bool isActive = true;
	const char* name;
	std::vector<Component*> components;
	GameObject(const char* _name = nullptr);
	~GameObject();
	void AddComponent(Component* comp);
	void RemoveComponent(Component* comp);
	void EmitComponentEvent(void(*eventFunction)(Component*));
	void StartComponents();
	void StartComponent(Component* comp);
	void UpdateComponents();
	void UpdateComponent(Component* comp);
};

