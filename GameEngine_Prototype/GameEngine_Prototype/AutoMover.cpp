#include "AutoMover.h"
#include "Transform.h"
#include "GameObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Time.h"
#include "ApplicationManager.h"
#include "RenderManager.h"
using namespace glm;

AutoMover::AutoMover() {}
AutoMover::AutoMover(float modDirIn) {
	modDir = modDirIn;
}
AutoMover::~AutoMover() {}

void AutoMover::Start()
{
	//moveSpeed = 2.5f;
	//gameObject->transform->model = translate(gameObject->transform->model, vec3(0.0f, 0.0f, -3.0f));
}

void AutoMover::Update()
{
	//glm::vec3 forward = gameObject->transform->getForwardDirection();
	//glm::vec3 up = gameObject->transform->getUpDirection
	float time = Time::getInstance().currentTime * moveSpeed;
	float delta = Time::getInstance().deltaTime * modDir;
	glm::vec3 pos = gameObject->transform->getPosition();
	gameObject->transform->Translate(glm::vec3(glm::sin(time) * delta, glm::sin(time*2) * delta, glm::cos(time) * delta));
}
