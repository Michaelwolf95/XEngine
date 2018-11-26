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


	gameObject->transform->TestEulerRotation(0, 0, 80);
	gameObject->transform->TestEulerRotation(0, 0, 90);
	gameObject->transform->TestEulerRotation(0, 0, 100);
	std::cout << atan2(0, 1) << std::endl;
	std::cout << asin(0) << std::endl;
	std::cout << atan2f( 2* cos(glm::radians(50.0f))*sin(glm::radians(50.0f)), 1- (2*sin(glm::radians(50.0f))*sin(glm::radians(50.0f)))) << std::endl;
	//std::cout << atan2f(0.98480775301f, 0.17364817767f) << std::endl;
	gameObject->transform->TestEulerRotation(0, 0, -80);
	gameObject->transform->TestEulerRotation(0, 0, -90);
	gameObject->transform->TestEulerRotation(0, 0, -100);
	std::cout << std::endl;

	gameObject->transform->TestEulerRotation(0, 80, 0);
	gameObject->transform->TestEulerRotation(0, 90, 0);
	gameObject->transform->TestEulerRotation(0, 100, 0);
	std::cout << atan2f(0, 1 - (2 * sin(glm::radians(50.0f))*sin(glm::radians(50.0f)))) << std::endl;
	std::cout << asin(2 * sin(glm::radians(50.0f))*cos(glm::radians(50.0f))) << std::endl;

	gameObject->transform->TestEulerRotation(180, 80, 180);

	gameObject->transform->TestEulerRotation(0, 180, 0);
	gameObject->transform->TestEulerRotation(0, 360, 0);

	//gameObject->transform->TestEulerRotation(0, -80, 0);
	//gameObject->transform->TestEulerRotation(0, -90, 0);
	//gameObject->transform->TestEulerRotation(0, -100, 0);
	//std::cout << std::endl;

	//gameObject->transform->TestEulerRotation(80,  0, 0);
	//gameObject->transform->TestEulerRotation(90,  0, 0);
	//gameObject->transform->TestEulerRotation(100, 0, 0);
	//gameObject->transform->TestEulerRotation(-80, 0, 0);
	//gameObject->transform->TestEulerRotation(-90, 0, 0);
	//gameObject->transform->TestEulerRotation(-100, 0, 0);

	std::cout << std::endl;
}

void TransformTester::Update()
{
	return;
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
