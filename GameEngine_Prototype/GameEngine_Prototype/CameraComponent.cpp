#include "CameraComponent.h"
#include "ApplicationManager.h"
#include "RenderManager.h"
#include "GameObject.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>

REGISTER_COMPONENT(CameraComponent, "CameraComponent")

CameraComponent::CameraComponent() 
{
	projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f),
		(float)ApplicationManager::config->screenWidth / (float)ApplicationManager::config->screenHeight,
		0.1f, 100.0f);
}

CameraComponent::~CameraComponent() 
{
	if (RenderManager::getInstance().getCurrentCamera() == this)
	{
		RenderManager::getInstance().setCurrentCamera(nullptr);
	}
}

glm::mat4 CameraComponent::getProjection()
{
	return projection;
}

glm::mat4 __stdcall CameraComponent::getView()
{
	//OutputDebugStringW(L"Getting CameraComponent View...\n");
	//std::cout << "Getting CameraComponent View..." << std::endl;
	glm::vec3 cameraPos = gameObject->transform->getPosition();
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + gameObject->transform->getForwardDirection(), gameObject->transform->getUpDirection());

	return view;
	//return (gameObject->transform->getMatrix4x4());
}

void CameraComponent::Start() {}
void CameraComponent::Update() {}
