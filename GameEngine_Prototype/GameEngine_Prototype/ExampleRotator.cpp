#include "ExampleRotator.h"
#include "GameObject.h"
#include "Time.h"
//#include <GLFW/glfw3.h>
#include "ApplicationManager.h"

ExampleRotator::ExampleRotator() {}
ExampleRotator::~ExampleRotator() {}

void ExampleRotator::Start()
{

}

void ExampleRotator::Update()
{
	glm::vec3 rot = gameObject->transform->getLocalRotationEuler();
	float deltaY = rotationSpeed * Time::deltaTime;

	// Currently broken API
	//glm::vec3 newRot = glm::vec3(rot.x, rot.y + deltaY, rot.z);
	//gameObject->transform->setLocalRotationEuler(newRot);

	float horizontal = 0;
	//Input::GetButtonHold(KEY_CODE) // return bool
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_LEFT) == GLFW_PRESS)
		horizontal += -1;
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_RIGHT) == GLFW_PRESS)
		horizontal += 1;

	deltaY *= horizontal;

	// Rotate using glm Rotate. This one DOESN'T break.
	gameObject->transform->model = glm::rotate(gameObject->transform->model,
		deltaY, glm::vec3(0,1,0));
}
