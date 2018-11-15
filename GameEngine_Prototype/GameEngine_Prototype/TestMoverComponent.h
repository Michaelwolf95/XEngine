#pragma once
#include "Component.h"
class TestMoverComponent :
	public Component
{
public:
	float moveSpeed = 2.5;
	TestMoverComponent();
	~TestMoverComponent();
	void Start() override;
	void Update() override;
};

