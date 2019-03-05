#include "FreeLookCameraController.h"
#include "Transform.h"
#include "GameObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Time.h"
#include "ApplicationManager.h"
#include "RenderManager.h"
#include "Input.h"
using namespace glm;

FreeLookCameraController::FreeLookCameraController() {}
FreeLookCameraController::~FreeLookCameraController() {}

void FreeLookCameraController::Start() {}

void FreeLookCameraController::Update()
{
	glm::vec3 forward = gameObject->transform->getForwardDirection();
	glm::vec3 up = gameObject->transform->getUpDirection(); 
	//glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
	//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	float deltaMove = moveSpeed * Time::deltaTime;
	if (Input::GetKey(GLFW_KEY_W))
		gameObject->transform->Translate(deltaMove * forward);
	if (Input::GetKey(GLFW_KEY_S))
		gameObject->transform->Translate(-deltaMove * forward);
	if (Input::GetKey(GLFW_KEY_A))
		gameObject->transform->Translate(-glm::normalize(glm::cross(forward, up)) * deltaMove);
	if (Input::GetKey(GLFW_KEY_D))
		gameObject->transform->Translate(glm::normalize(glm::cross(forward, up)) * deltaMove);
	if (Input::GetKey(GLFW_KEY_Q))
		gameObject->transform->Translate(-up * deltaMove);
	if (Input::GetKey(GLFW_KEY_E))
		gameObject->transform->Translate(up * deltaMove);

	if (Input::GetKeyDown(GLFW_KEY_P))
	{
		Input::ToggleCursor();
	}

	// TODO: Set Camera projection matrix using fov.
	//if (fov >= 1.0f && fov <= 45.0f)
	//	fov -= Input::getInstance().GetScrollOffsetY();
	//if (fov <= 1.0f)
	//	fov = 1.0f;
	//if (fov >= 45.0f)
	//	fov = 45.0f;

	float xoffset = Input::GetDeltaPosX();
	float yoffset = Input::GetDeltaPosY();

	xoffset *= lookSensitivity;
	yoffset *= lookSensitivity;

	glm::vec3 eulerRot = gameObject->transform->getLocalRotationEuler();
	float pitch = eulerRot.x + yoffset;
	float yaw = eulerRot.y + xoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	gameObject->transform->Rotate(glm::vec3(0, xoffset, 0.0f));
	gameObject->transform->Rotate(glm::vec3(yoffset, 0, 0.0f));
	//gameObject->transform->setLocalRotationEuler(pitch, yaw, eulerRot.z);
}
