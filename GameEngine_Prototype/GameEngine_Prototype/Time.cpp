#include "Time.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Time* Time::instance = nullptr;
float Time::deltaTime = 0.0f;
float Time::currentTime = 0.0f;

Time::Time(){}
Time::~Time() {}

Time * Time::CreateManager()
{
	if (instance != nullptr)
	{
		printf("DUPLICATE SINGLETON DETECTED");
		return NULL;
	}
	instance = new Time();

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

