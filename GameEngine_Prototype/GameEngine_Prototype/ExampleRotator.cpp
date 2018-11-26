#include "ExampleRotator.h"
#include "GameObject.h"
#include "Time.h"
#include "Input.h"

ExampleRotator::ExampleRotator() {}
ExampleRotator::~ExampleRotator() {}

void ExampleRotator::Start() {}

void ExampleRotator::Update()
{
	// Get the "horizontal" input for the arrow keys. 
	// Left is -1, Right is 1, and none is 0.
	float horizontal = 0;
	if (Input::GetKey(GLFW_KEY_LEFT))
		horizontal += -1;
	if (Input::GetKey(GLFW_KEY_RIGHT))
		horizontal += 1;

	// Get the change in rotation based on the input, speed, and change in time.
	float deltaY = horizontal * rotationSpeed * Time::deltaTime;

	// Get the current rotation of the object in degrees.
	glm::vec3 rot = gameObject->transform->getLocalRotationEuler();

	// Create a new rotation with y-rotation changed by deltaY.
	glm::vec3 newRot = glm::vec3(rot.x, rot.y + deltaY, rot.z);

	// Apply the new rotation to the transform.
	gameObject->transform->setLocalRotationEuler(newRot);
}
