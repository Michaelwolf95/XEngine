#include "TransformTester.h"
#include "GameObject.h"
#include "Time.h"
//#include <GLFW/glfw3.h>
#include "ApplicationManager.h"
#include "DebugUtility.h"

TransformTester::TransformTester() {}
TransformTester::~TransformTester() {}

void TransformTester::Start()
{
	glm::vec3 newRot = glm::vec3(0, 15, 0);
	//gameObject->transform->setLocalRotationEuler(newRot);
}

void TransformTester::Update()
{

	float deltaY = rotationSpeed * Time::deltaTime;
	float deltaZ = rotationSpeed * Time::deltaTime;

	float horizontal = 0;
	float vertical = 0;
	//Input::GetButtonHold(KEY_CODE) // return bool
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_LEFT) == GLFW_PRESS)
		horizontal += -1;
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_RIGHT) == GLFW_PRESS)
		horizontal += 1;
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_UP) == GLFW_PRESS)
		vertical += -1;
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_DOWN) == GLFW_PRESS)
		vertical += 1;

	deltaY *= horizontal;
	deltaZ *= vertical;

	glm::vec3 rot = gameObject->transform->getLocalRotationEuler();

	// Currently broken API
	glm::vec3 newRot = glm::vec3(rot.x, rot.y + deltaY, rot.z + deltaZ);
	gameObject->transform->setLocalRotationEuler(newRot);

	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (!isPressed)
		{
			gameObject->transform->printTransformMatrix();
			glm::vec3 rotDeg = gameObject->transform->getLocalRotationEuler();
			std::cout << "Rot:  (" << rotDeg.x << ", " << rotDeg.y << ", " << rotDeg.z << ")" << std::endl;
			glm::quat rotQuat = gameObject->transform->getLocalRotation();
			std::cout << "RotQ: (" << rotQuat.x << ", " << rotQuat.y << ", " << rotQuat.z << ", " << rotQuat.w << ")" << std::endl;
			
			//EngineDebug::PrintMatrix(gameObject->transform->getRotationMatrix());
			isPressed = true;
		}
	}
	else if (isPressed)
	{
		isPressed = false;
	}
}
