#pragma once
#include "Component.h"
class SimpleRotator : public Component
{
public:
	float xRotSpeed = 45;
	float yRotSpeed = 45;
	SimpleRotator();
	~SimpleRotator();
	void Start() override;
	void Update() override;
	SimpleRotator * DeepCopy() override;
};

