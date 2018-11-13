#include "TestMoverComponent.h"
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
using namespace glm;

glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

TestMoverComponent::TestMoverComponent() {}
TestMoverComponent::~TestMoverComponent() {}

void TestMoverComponent::Start()
{
	std::cout << "Start!" << std::endl;
	moveSpeed = 2.5f;
	gameObject->transform->model = translate(gameObject->transform->model, vec3(0.0f, 0.0f, -3.0f));
}

void TestMoverComponent::Update()
{
	float deltaMove = moveSpeed * Time::deltaTime;
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_W) == GLFW_PRESS)
		gameObject->transform->model = translate(gameObject->transform->model, deltaMove * cameraFront);
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_S) == GLFW_PRESS)
		gameObject->transform->model = translate(gameObject->transform->model, -deltaMove * cameraFront);
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_A) == GLFW_PRESS)
		gameObject->transform->model = translate(gameObject->transform->model, -glm::normalize(glm::cross(cameraFront, cameraUp)) * deltaMove);
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_D) == GLFW_PRESS)
		gameObject->transform->model = translate(gameObject->transform->model, glm::normalize(glm::cross(cameraFront, cameraUp)) * deltaMove);
}
