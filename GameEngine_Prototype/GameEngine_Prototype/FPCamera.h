#pragma once
#include "CameraComponent.h"
#include "Input.h"

class FPCamera :public CameraComponent
{
	float zoomSpeed = 15.0f;
	float panSpeed = 0.1f;
	float xRotSpeed = 5.0f;
	float yRotSpeed = 5.0f;
	glm::vec2 newPos;
	glm::vec2 lastPos;

public:
	void Update();
	FPCamera();
	~FPCamera();

	// add serialization
};

