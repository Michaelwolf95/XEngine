#pragma once
#include "Component.h"
class FreeLookCameraController : public Component
{
public:
	float moveSpeed = 2.5;
	bool firstMouse = true;
	float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float pitch = 0.0f;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;
	float fov = 45.0f;
	FreeLookCameraController();
	~FreeLookCameraController();
	void Start() override;
	void Update() override;
	//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

