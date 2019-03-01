#pragma once
#include "Component.h"
class AutoMover :
	public Component
{
public:
	float moveSpeed = 1.5;
	float modDir = 1;
	AutoMover();
	AutoMover(float modDir);
	~AutoMover();
	void Start() override;
	void Update() override;
};

