#include "ExampleRotator_James.h"
#include "GameObject.h"
#include "GameTime.h"
using namespace XEngine;

ExampleRotator_James::ExampleRotator_James() {}


ExampleRotator_James::~ExampleRotator_James() {}

void ExampleRotator_James::Start()
{

}

void ExampleRotator_James::Update()
{
	//glm::vec3 rot = gameObject->transform->getLocalRotationEuler();
	float deltaY = rotationSpeed * GameTime::deltaTime;
	//glm::vec3 newRot = glm::vec3(rot.x, rot.y + deltaY, rot.z);
	//gameObject->transform->getLocalRotationEuler(newRot);

	//Rotate using glm Totate. this one doesnt break
	gameObject->transform->Rotate(glm::vec3(0, deltaY, 0));
}
