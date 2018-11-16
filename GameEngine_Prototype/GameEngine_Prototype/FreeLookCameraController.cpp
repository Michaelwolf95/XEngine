#include "FreeLookCameraController.h"
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
#include "Input.h"
using namespace glm;


FreeLookCameraController::FreeLookCameraController()
{
}


FreeLookCameraController::~FreeLookCameraController()
{
}

void FreeLookCameraController::Start()
{
	//moveSpeed = 2.5f;
	//gameObject->transform->model = translate(gameObject->transform->model, vec3(0.0f, 0.0f, -3.0f));
}

void FreeLookCameraController::Update()
{
	//glm::vec3 forward = gameObject->transform->getForwardDirection();
	//glm::vec3 up = gameObject->transform->getUpDirection(); 
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	float deltaMove = moveSpeed * Time::deltaTime;
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_W) == GLFW_PRESS)
		gameObject->transform->model = translate(gameObject->transform->model, deltaMove * forward);
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_S) == GLFW_PRESS)
		gameObject->transform->model = translate(gameObject->transform->model, -deltaMove * forward);
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_A) == GLFW_PRESS)
		gameObject->transform->model = translate(gameObject->transform->model, -glm::normalize(glm::cross(forward, up)) * deltaMove);
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_D) == GLFW_PRESS)
		gameObject->transform->model = translate(gameObject->transform->model, glm::normalize(glm::cross(forward, up)) * deltaMove);
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_Q) == GLFW_PRESS)
		gameObject->transform->model = translate(gameObject->transform->model, -up * deltaMove);
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_E) == GLFW_PRESS)
		gameObject->transform->model = translate(gameObject->transform->model, up * deltaMove);

	if (fov >= 1.0f && fov <= 45.0f)
		fov -= Input::getInstance().yOffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;

	//if (firstMouse)
	//{
	//	lastX = xpos;
	//	lastY = ypos;
	//	firstMouse = false;
	//}

	float xoffset = Input::getInstance().xDeltaPos;// xpos - lastX;
	float yoffset = Input::getInstance().yDeltaPos; //lastY - ypos; // reversed since y-coordinates go from bottom to top

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	//glm::vec3 front;
	//front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	//front.y = sin(glm::radians(pitch));
	//front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	////cameraFront = glm::normalize(front);
	//glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	// Almost there!
	
	gameObject->transform->setLocalRotation(vec3(pitch, yaw, 0.0));
}

/*
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void FreeLookCameraController::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	//glm::vec3 front;
	//front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	//front.y = sin(glm::radians(pitch));
	//front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	////cameraFront = glm::normalize(front);

	gameObject->transform->setLocalRotation(vec3(pitch, yaw, 0.0));
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void FreeLookCameraController::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}
*/