#include "FreeLookCameraController.h"
#include "Transform.h"
#include "GameObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "GameTime.h"
#include "ApplicationManager.h"
#include "RenderManager.h"
#include "Input.h"
//using namespace glm;
using namespace XEngine;

REGISTER_COMPONENT(FreeLookCameraController, "FreeLookCameraController")

FreeLookCameraController::FreeLookCameraController() {}
FreeLookCameraController::~FreeLookCameraController() {}

void FreeLookCameraController::Start() {}

void FreeLookCameraController::Update()
{

	if (Input::GetKeyDown(GLFW_KEY_P))
	{
		Input::ToggleCursor();
	}
	glm::vec3 forward = this->gameObject->transform->getForwardDirection();
	glm::vec3 up = this->gameObject->transform->getUpDirection();


	// Move forward-back with Mouse Wheel.
	if (abs(Input::getInstance().GetScrollOffsetY()) > 0)
	{
		float deltaZoom = zoomSpeed * GameTime::deltaTime * Input::getInstance().GetScrollOffsetY();
		this->gameObject->transform->Translate(deltaZoom * forward);
	}

	switch (camMode)
	{
	default:
	case 0:
		if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
		{
			//clickPos = Input::GetMousePos();
			//lastDragPos = clickPos;
			//glfwSetInputMode(ApplicationManager::APP_WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			Input::ShowCursor(false);
			camMode = 1;
			break;
		}
		if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE) || (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && Input::GetKey(GLFW_KEY_LEFT_CONTROL)))
		{
			//clickPos = Input::GetMousePos();
			//lastDragPos = clickPos;
			//glfwSetInputMode(ApplicationManager::APP_WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			Input::ShowCursor(true);
			camMode = 2;
			break;
		}
		break;
	case 1:
		if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT))
		{
			//glm::vec2 currentDragPos = Input::GetMousePos();
			//glm::vec2 deltaPos = currentDragPos - lastDragPos;
			glm::vec2 deltaPos = Input::GetMouseDelta();
			//lastDragPos = currentDragPos;
			float deltaXRot = xRotSpeed * GameTime::deltaTime * deltaPos.x;
			float deltaYRot = yRotSpeed * GameTime::deltaTime * deltaPos.y;

			glm::vec3 localRot = gameObject->transform->getLocalRotationEuler();

			// ALTERNATIVE USING LOOKAT
			//glm::vec3 direction = gameObject->transform->getForwardDirection();
			//direction.x -= deltaYRot;
			//direction.y -= deltaXRot;
			//gameObject->transform->LookAt(gameObject->transform->getPosition() - direction);

			localRot = glm::vec3(localRot.x + deltaYRot, localRot.y + deltaXRot, localRot.z);
			gameObject->transform->setLocalRotationEuler(localRot);
		}
		else
		{
			//glfwSetInputMode(ApplicationManager::APP_WINDOW, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			Input::ShowCursor(true);
			camMode = 0;
		}
		break;
	case 2:
		if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_MIDDLE) || Input::GetMouseButton(GLFW_MOUSE_BUTTON_LEFT))
		{
			//glm::vec2 currentDragPos = Input::GetMousePos();
			//glm::vec2 deltaPos = currentDragPos - lastDragPos;
			glm::vec2 deltaPos = Input::GetMouseDelta();
			//lastDragPos = currentDragPos;
			float deltaXPan = panSpeed * GameTime::deltaTime * deltaPos.x;
			float deltaYPan = panSpeed * GameTime::deltaTime * deltaPos.y;// Input::GetDeltaPosY();

			glm::vec3 right = gameObject->transform->getRightDirection();
			//glm::vec3 up = gameObject->transform->getUpDirection();

			this->gameObject->transform->Translate(right * deltaXPan);
			this->gameObject->transform->Translate(up * deltaYPan);

		}
		else
		{
			//glfwSetInputMode(ApplicationManager::APP_WINDOW, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			Input::ShowCursor(true);
			camMode = 0;
		}

		break;
	}
}

void FreeLookCameraController::DrawInspector()
{
	ImGui::InputFloat("zoomSpeed", &zoomSpeed);
	ImGui::InputFloat("panSpeed", &panSpeed);
	ImGui::InputFloat("X RotSpeed", &xRotSpeed);
	ImGui::InputFloat("Y RotSpeed", &yRotSpeed);
}
