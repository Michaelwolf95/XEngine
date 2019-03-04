#include "EditorCamera.h"
#include "ApplicationManager.h"
#include "GameObject.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "RenderManager.h"
#include "SceneManager.h"
#include "Time.h"
#include "Input.h"
BOOST_CLASS_EXPORT_GUID(EditorCamera, "EditorCamera")

EditorCamera::EditorCamera()
{
	executeInEditMode = true;
	projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f),
		(float)ApplicationManager::config->screenWidth / (float)ApplicationManager::config->screenHeight,
		0.1f, 100.0f);
}

EditorCamera::~EditorCamera() 
{
	std::cout << "Deleting EditorCamera" << std::endl;
	if (RenderManager::getInstance().getCurrentCamera() == this)
	{
		RenderManager::getInstance().setCurrentCamera(nullptr);
	}
}

glm::mat4 EditorCamera::getProjection()
{
	return projection;
}

glm::mat4 __stdcall EditorCamera::getView()
{
	//std::cout << "Getting EditorCamera View..." << std::endl;
	glm::vec3 cameraPos = gameObject->transform->getPosition();
	return (glm::mat4) glm::lookAt(cameraPos, cameraPos + gameObject->transform->getForwardDirection(), gameObject->transform->getUpDirection());
}

void EditorCamera::Start() 
{
	if (ApplicationManager::getInstance().IsEditMode())
	{
		//RenderManager::getInstance().setCurrentCamera(this);
		isBeingUsed = true;
	}
}
void EditorCamera::Update()
{
	// Swap if edit mode changes.
	if (!isBeingUsed)
	{
		if (ApplicationManager::getInstance().IsEditMode())
		{
			//RenderManager::getInstance().setCurrentCamera(this);
			isBeingUsed = true;
		}
	}
	else
	{
		if (!ApplicationManager::getInstance().IsEditMode())
		{
			//RenderManager::getInstance().FindCameraInScene(SceneManager::getInstance().GetActiveScene().get());
			isBeingUsed = false;
		}
	}

	if (isBeingUsed)
	{
		//glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
		//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 forward = this->gameObject->transform->getForwardDirection();
		glm::vec3 up = this->gameObject->transform->getUpDirection();
		float moveSpeed = 5.0f;

		// Editor Camera Control.

		// TODO: Move forward-back with Mouse Wheel.

		if (abs(Input::getInstance().GetScrollOffsetY()) > 0)
		{
			float deltaZoom = moveSpeed * Time::deltaTime * Input::getInstance().GetScrollOffsetY();
			this->gameObject->transform->Translate(deltaZoom * forward);
		}

		// TODO: Pan with CTRL+Left Click (Or middle mouse)

		// TODO: Rotate with Right Click

		//std::cout << "Read Input: " << Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT) << std::endl;
		if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
		{
			std::cout << "Right click!" << std::endl;
		}

		//====================================================================
		//float deltaMove = moveSpeed * Time::deltaTime;

		//if (Input::GetKey(GLFW_KEY_UP))
		//	this->gameObject->transform->Translate(deltaMove * forward);
		//if (Input::GetKey(GLFW_KEY_DOWN))
		//	this->gameObject->transform->Translate(-deltaMove * forward);

		//if (Input::GetKey(GLFW_KEY_LEFT))
		//	this->gameObject->transform->Translate(-glm::normalize(glm::cross(forward, up)) * deltaMove);
		//if (Input::GetKey(GLFW_KEY_RIGHT))
		//	this->gameObject->transform->Translate(glm::normalize(glm::cross(forward, up)) * deltaMove);

		//if ((Input::GetKey(GLFW_KEY_LEFT_SHIFT) || Input::GetKey(GLFW_KEY_RIGHT_SHIFT)) == false)
		//{
		//	if (Input::GetKey(GLFW_KEY_UP))
		//		this->gameObject->transform->Translate(deltaMove * forward);
		//	if (Input::GetKey(GLFW_KEY_DOWN))
		//		this->gameObject->transform->Translate(-deltaMove * forward);
		//}
		//else // Pressing Shift
		//{
		//	if (Input::GetKey(GLFW_KEY_UP))
		//		this->gameObject->transform->Translate(up * deltaMove);
		//	if (Input::GetKey(GLFW_KEY_DOWN))
		//		this->gameObject->transform->Translate(-up * deltaMove);
		//}
	}
}