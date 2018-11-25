#include "ExampleRotator.h"
#include "GameObject.h"
#include "Time.h"
//#include <GLFW/glfw3.h>
#include "ApplicationManager.h"
#include "Input.h"

float ExampleRotator::rotationSpeed = 0.0f;
float ExampleRotator::deltaY = 0.0f;

ExampleRotator::ExampleRotator()
{
	std::cout << "ExampleRotator" << std::endl;
	Input::getInstance().keyEnabled(GLFW_KEY_LEFT, true);
	Input::getInstance().setKeyFunction(GLFW_KEY_LEFT, rotateLeft);
	Input::getInstance().keyEnabled(GLFW_KEY_RIGHT, true);
	Input::getInstance().setKeyFunction(GLFW_KEY_RIGHT, rotateRight);
}


ExampleRotator::~ExampleRotator()
{

}

void ExampleRotator::Start()
{

}

void ExampleRotator::Update()
{
	//std::cout << "Example Rotator Update" << std::endl;
	glm::vec3 rot = gameObject->transform->getLocalRotationEuler();
	rotationSpeed = 0.5f; // otherwise is equal to 10
	// TODO: find out why equal to 10 without reassignment.
	//deltaY = rotationSpeed * Time::deltaTime;


	//float horizontal = 0;
	//Input::GetButtonHold(KEY_CODE); // returns true or false

	if (Input::getInstance().isKeyPressed(GLFW_KEY_LEFT) == true)
	{
		Input::getInstance().callKeyFunction(GLFW_KEY_LEFT);
	}
	if (Input::getInstance().isKeyPressed(GLFW_KEY_RIGHT) == true)
	{
		Input::getInstance().callKeyFunction(GLFW_KEY_RIGHT);
	}
	//if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_LEFT) == GLFW_PRESS)
	//	horizontal += -1;
	//if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_RIGHT) == GLFW_PRESS)
	//	horizontal += 1;
	//deltaY *= horizontal;

	//glm::vec3 newRot = glm::vec3(rot.x, rot.y+deltaY, rot.z);
	gameObject->transform->Rotate(glm::vec3(0, deltaY*rotationSpeed, 0));
	deltaY = 0.0f;
}

void ExampleRotator::rotateLeft()
{
	//std::cout << "rotateLeft" << std::endl;
	//glm::vec3 rot = gameObject->transform->getLocalRotationEuler();
	deltaY = rotationSpeed;// *Time::deltaTime;

	//float horizontal = deltaY;
	////Input::GetButtonHold(KEY_CODE); // returns true or false
	//horizontal *= -1;
	deltaY *= -1;

	//glm::vec3 newRot = glm::vec3(rot.x, rot.y + deltaY, rot.z);
	//gameObject->transform->Rotate(glm::vec3(0, deltaY, 0));
}

void ExampleRotator::rotateRight()
{
	deltaY = rotationSpeed;

	//float horizontal = deltaY;
	////Input::GetButtonHold(KEY_CODE); // returns true or false
	//horizontal += 1;
	deltaY *= 1;
}