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

BOOST_CLASS_EXPORT_GUID(EditorCamera, "EditorCamera")

EditorCamera::EditorCamera()
{
	projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f),
		(float)ApplicationManager::config->screenWidth / (float)ApplicationManager::config->screenHeight,
		0.1f, 100.0f);
}

EditorCamera::~EditorCamera() {}

glm::mat4 EditorCamera::getProjection()
{
	return projection;
}

mat4 EditorCamera::getView()
{
	glm::vec3 cameraPos = gameObject->transform->getPosition();
	return glm::lookAt(cameraPos, cameraPos + gameObject->transform->getForwardDirection(), gameObject->transform->getUpDirection());
	//return (gameObject->transform->getMatrix4x4());
}

void EditorCamera::Start() 
{
	executeInEditMode = true;
	if (ApplicationManager::getInstance().IsEditMode())
	{
		RenderManager::getInstance().setCurrentCamera(this);

	}
}
void EditorCamera::Update()
{
	// Swap if edit mode changes.
	if (!isBeingUsed)
	{
		if (ApplicationManager::getInstance().IsEditMode())
		{
			RenderManager::getInstance().setCurrentCamera(this);
			isBeingUsed = true;
		}
	}
	else
	{
		if (!ApplicationManager::getInstance().IsEditMode())
		{
			RenderManager::getInstance().FindCameraInScene(SceneManager::getInstance().GetActiveScene());
			isBeingUsed = false;
		}
	}
}