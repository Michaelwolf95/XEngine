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
#include "Input.h"
#include "ApplicationManager.h"
#include "RenderManager.h"
using namespace glm;

// "'boost::serialization::guid': function must return a value" error if these macros don't have the string value.
REGISTER_COMPONENT(TestMoverComponent, "TestMoverComponent")

TestMoverComponent::TestMoverComponent() {}
TestMoverComponent::~TestMoverComponent() {}

void TestMoverComponent::Start()
{
	//moveSpeed = 2.5f;
	//gameObject->transform->model = translate(gameObject->transform->model, vec3(0.0f, 0.0f, -3.0f));
}

void TestMoverComponent::Update()
{
	//glm::vec3 forward = gameObject->transform->getForwardDirection();
	//glm::vec3 up = gameObject->transform->getUpDirection(); 
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	float deltaMove = moveSpeed * Time::deltaTime;
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_W) == GLFW_PRESS)
		gameObject->transform->Translate(deltaMove * forward);
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_S) == GLFW_PRESS)
		gameObject->transform->Translate(-deltaMove * forward);
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_A) == GLFW_PRESS)
		gameObject->transform->Translate(-glm::normalize(glm::cross(forward, up)) * deltaMove);
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_D) == GLFW_PRESS)
		gameObject->transform->Translate(glm::normalize(glm::cross(forward, up)) * deltaMove);
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_Q) == GLFW_PRESS)
		gameObject->transform->Translate(-up * deltaMove);
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_E) == GLFW_PRESS)
		gameObject->transform->Translate(up * deltaMove);

	//if (Input::GetKeyDown(GLFW_KEY_P) && (Input::GetKey(GLFW_KEY_LEFT_CONTROL) || Input::GetKey(GLFW_KEY_RIGHT_CONTROL)))
	//{
	//	// Output current position.
	//	glm::vec3 pos = this->gameObject->transform->getPosition();
	//	std::cout << "Pos: (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
	//}
}
