#include "CameraComponent.h"
#include "ApplicationManager.h"
#include "GameObject.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>

CameraComponent::CameraComponent() 
{
	projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f),
		(float)ApplicationManager::config->screenWidth / (float)ApplicationManager::config->screenHeight,
		0.1f, 100.0f);
}

CameraComponent::~CameraComponent() {}

mat4 CameraComponent::getProjection()
{
	return projection;
}

mat4 CameraComponent::getView()
{
	// Make sure the model is never scaled?
	return (gameObject->transform->model);
}

void CameraComponent::Start() {}
void CameraComponent::Update() {}
