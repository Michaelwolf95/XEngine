#pragma once
#include "Component.h"
class AutoMover :
	public Component
{
public:
	float moveSpeed = 1.5;
	float modDir = 1;
	float xMod = 1.0f;
	float yMod = 1.0f;
	float zMod = 1.0f;

	AutoMover();
	AutoMover(float modDir);
	AutoMover(float modDirIn, float xModIn, float yModIn, float zModIn);
	~AutoMover();
	void Start() override;
	void Update() override;
};

