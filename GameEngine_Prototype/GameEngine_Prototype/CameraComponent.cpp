#include "CameraComponent.h"
#include "ApplicationManager.h"
#include "GameObject.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>

BOOST_CLASS_EXPORT_GUID(CameraComponent, "CameraComponent")

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
	glm::vec3 cameraPos = gameObject->transform->getPosition();
	return glm::lookAt(cameraPos, cameraPos + gameObject->transform->getForwardDirection(), gameObject->transform->getUpDirection());
	//return (gameObject->transform->getMatrix4x4());
}

void CameraComponent::Start() {}
void CameraComponent::Update() {}
