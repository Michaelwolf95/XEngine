#pragma once
#include "Component.h"
#include <glm/glm.hpp>
class FreeLookCameraController : public Component
{
public:
	float zoomSpeed = 15.0f;
	float xRotSpeed = 0.05f;
	float yRotSpeed = 0.05f;
	float panSpeed = 0.1f;
	//float fov = 45.0f;
	FreeLookCameraController();
	~FreeLookCameraController();
	void Start() override;
	void Update() override;
private:
	unsigned int camMode;
	glm::vec2 clickPos;
	glm::vec2 lastDragPos;
	
};

