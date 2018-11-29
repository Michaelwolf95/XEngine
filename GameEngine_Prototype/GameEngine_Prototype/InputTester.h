#pragma once
#include "Component.h"

class InputTester : public Component
{
public:
	float rotationSpeed;
	int sampleSize = 0;
	InputTester();
	~InputTester();
	void Start() override;
	void Update() override;
};

