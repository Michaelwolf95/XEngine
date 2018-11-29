#pragma once
#include "Component.h"
class FreeLookCameraController : public Component
{
public:
	float moveSpeed = 2.5;
	float lookSensitivity = 0.1f;
	//float fov = 45.0f;
	FreeLookCameraController();
	~FreeLookCameraController();
	void Start() override;
	void Update() override;
};

