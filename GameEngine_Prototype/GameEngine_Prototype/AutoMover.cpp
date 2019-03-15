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

AutoMover::AutoMover(float modDirIn, float xModIn, float yModIn, float zModIn) {
	if (xModIn != 0) xModIn = 1/xModIn;
	if (yModIn != 0) yModIn = 1/yModIn;
	if (zModIn != 0) zModIn = 1/zModIn;

	modDir = modDirIn;
	xMod = xModIn;
	yMod = yModIn;
	zMod = zModIn;
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
	gameObject->transform->Translate(glm::vec3(glm::sin(time*xMod) * delta, 
									glm::sin(time*yMod) * delta, 
									glm::cos(time*zMod) * delta));
}
