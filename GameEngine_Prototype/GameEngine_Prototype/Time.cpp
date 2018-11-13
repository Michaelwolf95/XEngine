#include "Time.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

float Time::deltaTime = 0.0f;
float Time::currentTime = 0.0f;

Time::Time(){}

Time * Time::CreateManager()
{
	Time* instance = &Time::getInstance();

	instance->Init();

	return instance;
}

void Time::Init()
{
	currentTime = glfwGetTime();
	timeLastFrame = 0.0f;
	deltaTime = 0.0f;
}

void Time::UpdateTime()
{
	currentTime = glfwGetTime();
	deltaTime = currentTime - timeLastFrame;
	timeLastFrame = currentTime;
}

