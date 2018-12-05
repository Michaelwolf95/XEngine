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
	//std::cout << "lRotERad:(" << glm::degrees(eulerRot.x) << ", " << glm::degrees(eulerRot.y) << ", " << glm::degrees(eulerRot.z) << ")" << std::endl;
	float pitch = glm::degrees(eulerRot.x) + yoffset;
	float yaw = glm::degrees(eulerRot.y) + xoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	//gameObject->transform->setLocalRotationEuler(pitch, yaw, eulerRot.z);

	//gameObject->transform->Rotate(glm::vec3(0, xoffset, 0.0f));
	//gameObject->transform->Rotate(glm::vec3(yoffset, 0, 0.0f));

	//std::cout << "lRotE:  (" << pitch << ", " << yaw << ", " << glm::degrees(eulerRot.z) << ")" << std::endl;

	//glm::vec3 fwd = gameObject->transform->getForwardDirection();
	//glm::quat rQ(glm::vec3(yoffset, xoffset, 0.0f));
	//glm::vec3 newForward = rQ * fwd; // * glm::conjugate(rQ)
	//glm::vec3 lookPos = gameObject->transform->getPosition() + newForward;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);
	glm::vec3 lookPos = gameObject->transform->getPosition() + front;

	
	//std::cout << "lPos:  (" << lookPos.x << ", " << lookPos.y << ", " << lookPos.z << ")" << std::endl;
	//gameObject->transform->LookAt(lookPos, glm::vec3(0, 1, 0));

	gameObject->transform->Rotate(glm::vec3(yoffset, xoffset, 0));

	if (Input::GetKeyDown(GLFW_KEY_R))
	{
		gameObject->transform->printTransformMatrix();
		std::cout << "Camera Rot:" << std::endl;
		glm::vec3 rotDeg = gameObject->transform->getLocalRotationEuler();
		std::cout << "Rot:  (" << rotDeg.x << ", " << rotDeg.y << ", " << rotDeg.z << ")" << std::endl;
		glm::quat rotQuat = gameObject->transform->getLocalRotation();
		std::cout << "RotQ: (" << rotQuat.x << ", " << rotQuat.y << ", " << rotQuat.z << ", " << rotQuat.w << ")" << std::endl;
		glm::vec3 scale = gameObject->transform->getLocalScale();
		std::cout << "Scale:  (" << scale.x << ", " << scale.y << ", " << scale.z << ")" << std::endl;
	}
}
