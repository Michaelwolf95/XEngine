#pragma once
#include "Component.h"
#include "Camera.h"
class CameraSwapper : public Component
{
public:
	Camera* cam1;
	Camera* cam2;
	Camera* cam3;
	CameraSwapper();
	~CameraSwapper();
	void Start() override;
	void Update() override;
};

