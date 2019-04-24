#include "CameraComponent.h"
#include "ApplicationManager.h"
#include "RenderManager.h"
#include "AssetManager.h"
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
	//initGizmoRenderData();

	gizmoDrawer = std::shared_ptr<GizmoSpriteDrawer>(new GizmoSpriteDrawer("Editor/Gizmos/CameraGizmo.png"));
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
	glm::mat4 view = glm::lookAt(cameraPos, 
		cameraPos + glm::normalize(gameObject->transform->getForwardDirection()), 
		gameObject->transform->getUpDirection());

	return view;
}

void CameraComponent::Start() {}
void CameraComponent::Update() {}


void CameraComponent::OnDrawGizmos()
{
	gizmoDrawer->Draw(this->gameObject->transform->getPosition());
}

void CameraComponent::DrawInspector()
{
	ImGui::PushID(this);
	ImGui::ColorEdit4("Color", (float*)&clearColor);
	ImGui::PopID();
}

