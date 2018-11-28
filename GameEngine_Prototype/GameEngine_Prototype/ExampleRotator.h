#pragma once
#include "Component.h"

class ExampleRotator : public Component
{
public:
	static float rotationSpeed;// = 25.0f;
	static float deltaY;
	ExampleRotator();
	~ExampleRotator();
	void Start() override;
	void Update() override;

	static void rotateLeft();
	static void rotateRight();
};

