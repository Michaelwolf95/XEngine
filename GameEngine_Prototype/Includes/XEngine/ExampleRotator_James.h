#pragma once
#include "Component.h"

class ExampleRotator_James : public Component
{
public:
	float rotationSpeed = 25;
	ExampleRotator_James();
	~ExampleRotator_James();
	void Start() override;
	void Update() override;
	ExampleRotator_James * DeepCopy() override;
};

