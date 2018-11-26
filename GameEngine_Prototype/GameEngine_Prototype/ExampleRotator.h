#pragma once
#include "Component.h"

class ExampleRotator : public Component
{
public:
	float rotationSpeed = 25;
	bool isPressed = false;
	ExampleRotator();
	~ExampleRotator();
	void Start() override;
	void Update() override;
};
