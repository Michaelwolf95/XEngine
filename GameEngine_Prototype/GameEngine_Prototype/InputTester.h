#pragma once
#include "Component.h"

class InputTester : public Component
{
public:
	float rotationSpeed;
	InputTester();
	~InputTester();
	void Start() override;
	void Update() override;
};

