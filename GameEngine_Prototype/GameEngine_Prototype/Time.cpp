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

void Time::updateTime()
{
	currentTime = glfwGetTime();
	deltaTime = currentTime - timeLastFrame;
	timeLastFrame = currentTime;
}

void Time::UpdateTime()
{
	Time::getInstance().updateTime();
}

void Time::getFPS(void(*callback)(float))
{
	fps.getFPS(callback);
	// remote function assigned to callback is called
}

void Time::toggleFPS()
{
	fps.isCounting = !fps.isCounting;
}

void Time::ToggleFPS()
{
	Time::getInstance().toggleFPS();
}

void Time::ModSampleSize(int &sample_sz)
{
	Time::getInstance().fps.modSampleSize(sample_sz);
}

void Time::GetFPS(void(*callback)(float))
{
	Time::getInstance().getFPS(callback);
	Time::getInstance().fps.doFPSCallback();
}

