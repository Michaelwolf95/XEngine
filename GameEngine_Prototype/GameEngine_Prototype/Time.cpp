#include "Time.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

float Time::deltaTime = 0.0f;
float Time::currentTime = 0.0f;

Time::Time(){}

void(*getFPSCallback)(float fps) = NULL;

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
	if (isCounting)
	{
		deltaTimeSumOf60 += Time::getInstance().deltaTime;
		//if (++counter % mod == 0)
		//{
			getFPSCallback(1 / (deltaTimeSumOf60 /*/ mod*/)); // crashes here when F is first key
			isCounting = false;
			getFPSCallback = NULL;
		//}
	}
}

void Time::getFPS(void(*callback)(float))
{
	counter = 0;
	deltaTimeSumOf60 = 0.f;
	getFPSCallback = callback;
	// remote function assigned to callback is called
}

void Time::toggleFPS()
{
	isCounting = !isCounting;
}

void Time::ToggleFPS()
{
	Time::getInstance().toggleFPS();
}

void Time::GetFPS(void(*callback)(float))
{
	Time::getInstance().isCounting = true;
	Time::getInstance().getFPS(callback);
}

