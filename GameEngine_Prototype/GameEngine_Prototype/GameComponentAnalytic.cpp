#include "GameComponentAnalytic.h"
#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "ApplicationManager.h"
#include "AssetManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Material.h"
#include "LightComponent.h"
#include "SimpleSprite.h"
#include "SimpleModel.h"
#include "SimpleModelComponent.h"
#include "SimpleRotator.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "Time.h"
#include "InputTester.h"
#include "Input.h"
#include "Analytics.h"
#include <string> 

//CSVMaker analytic = CSVMaker("componentFPS");

GameComponentAnalytic::GameComponentAnalytic()
{
	//objectNum = num;
}

GameComponentAnalytic::~GameComponentAnalytic()
{
	//analytic.Close();
}

void logFPSComponent(float fps)
{
	//analytic.Write(std::to_string(num), fps);
	//componentTest << fps << std::endl;
	//Time::getInstance().GetFPS(printFPS);
}

void printFPSComponent(float fps)
{
	std::cout << fps << std::endl;
}

void GameComponentAnalytic::Start()
{
	
	
	/*
	srand(time(NULL));

	float CUBE_VERTS[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 0 L Bottom Back
	0.5f, -0.5f, -0.5f,   1.0f, 0.0f, // 1 R Bottom Back
	0.5f,  0.5f, -0.5f,   1.0f, 1.0f, // 2 R Top Back
	0.5f,  0.5f, -0.5f,   1.0f, 1.0f, // 2 R Top Back
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 3 L Top Back
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 0 L Bottom Back

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,   1.0f, 1.0f, //
	0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, //
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,   0.0f, 1.0f, //
	0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,   1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,   1.0f, 0.0f, //
	0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,   1.0f, 0.0f, //
	0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	unsigned int CUBE_INDICES[] = {
	0, 1, 2,	0, 2, 3,      // front
	4, 5, 6,	4, 6, 7,      // back
	8, 9, 10,	8, 10, 11,    // top
	12, 13, 14, 12, 14, 15,   // bottom
	16, 17, 18, 16, 18, 19,   // right
	20, 21, 22, 20, 22, 23,   // left
	};

	*/

	//Scene* scene = SceneManager::getInstance().GetActiveScene();
	//Shader* modelShader = new Shader("model.vs", "model.fs");
	//Material* modelMaterial = new Material(modelShader);
	//modelMaterial->LoadTexture("textures/container.jpg"); //change model here if needed for different test

	//int range = 10;
	//loop to create more gameObjects
	//for (int i = 1; i < 50; i++)
	//{
	//	GameObject* c = scene->CreateGameObject("Cube");
	//	SimpleModelComponent* m = new SimpleModelComponent(CUBE_VERTS, 36, 5, CUBE_INDICES, 36, modelMaterial);
	//	c->AddComponent(m);
	//	c->transform->setLocalPosition((rand() % range) + 1, (rand() % range) + 1, (rand() % range) + 1);
		//c->AddComponent(new ExampleRotator()); //rotating all the GameObjects
	//}


}

void GameComponentAnalytic::Update()
{

	if (Input::GetKeyDown(GLFW_KEY_F)) Time::ToggleFPS();
	// Code that happens every frame.
	fpsSample(sampleSize);

	PrintFPS();
}

void GameComponentAnalytic::fpsSample(int &sample_sz)
{
	if (sample_sz-- > 0)
	{
		Time::GetFPS(logFPSComponent);
	}
}

void GameComponentAnalytic::PrintFPS()
{
	if (Time::getInstance().fps.isCounting)
		Time::GetFPS(printFPSComponent);
}
