#pragma once
#include "Component.h"

class TransformTester : public Component
{
public:
	float rotationSpeed = 25;
	bool isPressed = false;
	TransformTester();
	~TransformTester();
	void Start() override;
	void Update() override;
};

