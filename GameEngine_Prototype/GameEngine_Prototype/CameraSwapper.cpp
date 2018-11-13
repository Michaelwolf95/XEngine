#include "CameraSwapper.h"
#include "ApplicationManager.h"
#include "RenderManager.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

CameraSwapper::CameraSwapper() {}
CameraSwapper::~CameraSwapper() {}

void CameraSwapper::Start()
{
}

void CameraSwapper::Update()
{
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_1) == GLFW_PRESS)
	{
		if (RenderManager::getInstance().getCurrentCamera() != cam1)
		{
			RenderManager::getInstance().setCurrentCamera(cam1);
		}
	}
	else if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_2) == GLFW_PRESS)
	{
		if (RenderManager::getInstance().getCurrentCamera() != cam2)
		{
			RenderManager::getInstance().setCurrentCamera(cam2);
		}
	}
	else if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_3) == GLFW_PRESS)
	{
		if (RenderManager::getInstance().getCurrentCamera() != cam3)
		{
			RenderManager::getInstance().setCurrentCamera(cam3);
		}
	}
}
