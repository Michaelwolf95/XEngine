#include "ExampleRotator.h"
#include "GameObject.h"
#include "Time.h"
#include "Input.h"
#include "ApplicationManager.h"


ExampleRotator::ExampleRotator() {}
ExampleRotator::~ExampleRotator() {}

void ExampleRotator::Start(){}

void ExampleRotator::Update()
{
	float deltaY = rotationSpeed * Time::deltaTime;

	float horizontal = 0;
	if (Input::GetKey(GLFW_KEY_LEFT))
		horizontal += -1;
	if (Input::GetKey(GLFW_KEY_RIGHT))
		horizontal += 1;

	deltaY *= horizontal;

	glm::vec3 rot = gameObject->transform->getLocalRotationEuler();

	glm::vec3 newRot = glm::vec3(rot.x, rot.y + deltaY, rot.z);
	gameObject->transform->setLocalRotationEuler(newRot);

}
