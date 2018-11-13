#pragma once
//#include "GameObject.h" // Circular dependency - wont compile
class GameObject; // Use a "forward declaration" instead.

class Component
{
public:
	GameObject* owner; // Void* to resolve circular dependency issues.
	Component();
	~Component();
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void OnDestroy() = 0;
};

