#include "ExampleRotator_James.h"
#include "GameObject.h"
#include "Time.h"


ExampleRotator_James::ExampleRotator_James() {}


ExampleRotator_James::~ExampleRotator_James() {}

void ExampleRotator_James::Start()
{

}

void ExampleRotator_James::Update()
{
	glm::vec3 rot = gameObject->transform->getLocalRotationEuler();
	float deltaY = rotationSpeed * Time::deltaTime;
	glm::vec3 newRot = glm::vec3(rot.x, rot.y + deltaY, rot.z);
	//gameObject->transform->getLocalRotationEuler(newRot);

	//Rotate using glm Totate. this one doesnt break
	gameObject->transform->model = glm::rotate(gameObject->transform->model, deltaY, glm::vec3(0, 1, 0));
}
