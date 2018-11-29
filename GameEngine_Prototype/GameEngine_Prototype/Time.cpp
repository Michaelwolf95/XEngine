#include "Time.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

float Time::deltaTime = 0.0f;
float Time::currentTime = 0.0f;

Time::Time(){}

void(*getFPSCallback)(float) = NULL;

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

		if (++counter % mod == 0)
		{
			//std::cout << 1 / (deltaTimeSumOf60 / mod) << " frames/sec" << std::endl;
			//counter = 0;
			//iscounting = false;
			isCounting = false;
			getFPSCallback(1 / (deltaTimeSumOf60 / mod));
			//getFPSCallback = NULL;
		}
	}
}



void Time::GetFPS(void(*callback)(float))
{
	isCounting = true;
	counter = 0;
	deltaTimeSumOf60 = 0.f;
	getFPSCallback = callback;
	//callback(10);
}

