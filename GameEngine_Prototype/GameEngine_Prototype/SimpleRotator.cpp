#include "SimpleRotator.h"
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


SimpleRotator::SimpleRotator() {}
SimpleRotator::~SimpleRotator() {}

void SimpleRotator::Start()
{
}

void SimpleRotator::Update()
{
	float horz = 0;
	float vert = 0;
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_UP) == GLFW_PRESS)
		vert += 1;
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_DOWN) == GLFW_PRESS)
		vert -= 1;
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_RIGHT) == GLFW_PRESS)
		horz += 1;
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_LEFT) == GLFW_PRESS)
		horz -= 1;

	glm::vec3 rot = gameObject->transform->getLocalRotationEuler();
	rot.x += xRotSpeed * Time::deltaTime * vert;
	rot.y += yRotSpeed * Time::deltaTime * horz;
	gameObject->transform->setLocalRotationEuler(rot);
	//rot = gameObject->transform->getLocalRotationEuler();
	//std::cout << "Rot:(" << rot.x << ", " << rot.y << ", " << rot.z << ")" << std::endl;

}
