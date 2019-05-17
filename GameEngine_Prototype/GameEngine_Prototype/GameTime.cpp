#include "GameTime.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace XEngine
{
	float GameTime::deltaTime = 0.0f;
	float GameTime::currentTime = 0.0f;

	GameTime::GameTime() {}

	GameTime * GameTime::CreateManager()
	{
		GameTime* instance = &GameTime::getInstance();
		instance->Init();
		return instance;
	}

	void GameTime::Init()
	{
		currentTime = (float)glfwGetTime();
		timeLastFrame = 0.0f;
		deltaTime = 0.0f;
	}

	void GameTime::updateTime()
	{
		currentTime = (float)glfwGetTime();
		deltaTime = currentTime - timeLastFrame;
		timeLastFrame = currentTime;
	}

	void GameTime::UpdateTime()
	{
		GameTime::getInstance().updateTime();
	}

	void GameTime::getFPS(void(*callback)(float))
	{
		fps.getFPS(callback);
		// remote function assigned to callback is called
	}

	void GameTime::toggleFPS()
	{
		fps.isCounting = !fps.isCounting;
	}

	void GameTime::ToggleFPS()
	{
		GameTime::getInstance().toggleFPS();
	}

	void GameTime::ModSampleSize(int &sample_sz)
	{
		GameTime::getInstance().fps.modSampleSize(sample_sz);
	}

	void GameTime::GetFPS(void(*callback)(float))
	{
		GameTime::getInstance().getFPS(callback);
		GameTime::getInstance().fps.doFPSCallback();
	}

	bool GameTime::IsCounting()
	{
		return GameTime::getInstance().fps.isCounting;
	}

	void GameTime::SetSampleSetSize(int sample_set_sz)
	{
		GameTime::getInstance().fps.sampleSetSize = sample_set_sz;
	}

}