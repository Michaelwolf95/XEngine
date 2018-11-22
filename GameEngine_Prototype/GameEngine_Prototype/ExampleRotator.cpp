#include "ExampleRotator.h"
#include "GameObject.h"
#include "Time.h"
//#include <GLFW/glfw3.h>
#include "ApplicationManager.h"
#include "DebugUtility.h"

ExampleRotator::ExampleRotator() {}
ExampleRotator::~ExampleRotator() {}

void ExampleRotator::Start()
{

}

void ExampleRotator::Update()
{
	glm::vec3 rot = gameObject->transform->getLocalRotationEuler();
	float deltaY = rotationSpeed * Time::deltaTime;

	float horizontal = 0;
	//Input::GetButtonHold(KEY_CODE) // return bool
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_LEFT) == GLFW_PRESS)
		horizontal += -1;
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_RIGHT) == GLFW_PRESS)
		horizontal += 1;

	deltaY *= horizontal;

	// Currently broken API
	glm::vec3 newRot = glm::vec3(rot.x, rot.y + deltaY, rot.z);
	gameObject->transform->setLocalRotationEuler(newRot);

	// Rotate using glm Rotate. This one DOESN'T break.
	//gameObject->transform->Rotate(glm::vec3(0, deltaY, 0));

	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (!isPressed)
		{
			gameObject->transform->printTransformMatrix();
			glm::vec3 rotDeg = gameObject->transform->getLocalRotationEuler();
			std::cout << "Rot:  (" << rotDeg.x << ", " << rotDeg.y << ", " << rotDeg.z << ")" << std::endl;
			glm::vec3 scale = gameObject->transform->getLocalScale();
			std::cout << "Scale:(" << scale.x << ", " << scale.y << ", " << scale.z << ")" << std::endl;
			
			//EngineDebug::PrintMatrix(gameObject->transform->getRotationMatrix());
			isPressed = true;
		}
	}
	else if (isPressed)
	{
		isPressed = false;
	}
}
