#include "ExampleRotator.h"
#include "GameObject.h"
#include "Time.h"
//#include <GLFW/glfw3.h>
#include "ApplicationManager.h"
#include "Input.h"



ExampleRotator::ExampleRotator()
{

}


ExampleRotator::~ExampleRotator()
{

}

void ExampleRotator::Start()
{

}

void ExampleRotator::Update()
{
	glm::vec3 rot = gameObject->transform->getLocalRotationEuler();
	float deltaY = rotationSpeed * Time::deltaTime;

	float horizontal = 0;
	//Input::GetButtonHold(KEY_CODE); // returns true or false
	for (int i = 0; i < sizeof(Input::getInstance().keys) / sizeof(Input::KeyPressed); i++)
	{
		std::cout << i << std::endl;

	}
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_LEFT) == GLFW_PRESS)
		horizontal += -1;
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_RIGHT) == GLFW_PRESS)
		horizontal += 1;
	//TODO Ttestst
	deltaY *= horizontal;

	glm::vec3 newRot = glm::vec3(rot.x, rot.y+deltaY, rot.z);
	gameObject->transform->Rotate(glm::vec3(0, deltaY, 0));
}
