#pragma once
#include "Component.h"
class TestMoverComponent :
	public Component
{
public:
	float moveSpeed;
	TestMoverComponent();
	~TestMoverComponent();
	void Start() override;
	void Update() override;
};

