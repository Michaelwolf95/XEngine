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
#include "GameTime.h"
#include "InputTester.h"
#include "Input.h"
#include "Analytics.h"
#include <string> 
#include "ApplicationManager.h"
using namespace XEngine;

CSVMaker GameComponentAnalytic::csv = CSVMaker("component");
int GameComponentAnalytic::objectNum = 1;

// default constructor
GameComponentAnalytic::GameComponentAnalytic()
{
	objectNum = 1;
}

// constructor with defined number of components and size of fps samples
GameComponentAnalytic::GameComponentAnalytic(int num)
{
	objectNum = num;
}

// decontructor closes the csv file
GameComponentAnalytic::~GameComponentAnalytic()
{
	csv.Close();
}

// Print fps (based on InputTester)
void printFPSComponent(float fps)
{
	std::cout << fps << std::endl;
}

// Writes into the csv files (based on InputTester)
void logFPSComponent(float fps)
{
	GameComponentAnalytic::csv.Write(GameComponentAnalytic::objectNum, fps);
}

void GameComponentAnalytic::Start()
{
	// get only gameobject in scene
	Scene_ptr scene = SceneManager::getInstance().GetActiveScene();
	GameObject_ptr c = scene->rootGameObjects[0];

	//Shader* modelShader = new Shader("model.vs", "model.fs");
	//Material* modelMaterial = new Material(modelShader);
	//modelMaterial->LoadTexture("textures/container.jpg"); //change model here if needed for different test

	//int range = 10;
	//loop to create more gameObjects
	for (int i = 0; i < objectNum; i++)
	{
		//c->AddComponent(new LightComponent(glm::vec3(0.2f)));
		std::shared_ptr<SimpleRotator> sr(new SimpleRotator());
		c->AddComponent(sr);
		//c->AddComponent(std::make_shared<SimpleRotator>(new SimpleRotator()));
	}
}

void GameComponentAnalytic::Update()
{
	if (Input::GetKeyDown(GLFW_KEY_F)) GameTime::ToggleFPS();

	// Press R to print into the csv files
	// Wait a couple of seconds if there are a lot of components
	if (Input::GetKeyDown(GLFW_KEY_R)) {
		int framesPerSampleSize = 100; // **number of samples per FPS sample
		GameTime::SetSampleSetSize(framesPerSampleSize); // **set the number of samples per FPS sample. 
		GameTime::ModSampleSize(SAMPLE_SIZE = 25);	// set to take 25 samples
												// each sample size is average with the number of frames provided by modSampleSize
	}
	fpsSample(SAMPLE_SIZE); // **modified sample size is passed to custom function 
	//PrintFPS(); // **indefinitely prints FPS to console if isPrinting is true. Separate feature from fps sampling. Do not use if you only want to log fps samples to file.
}

// calls function to write into csv file for the size amount (based on InputTester)
void GameComponentAnalytic::fpsSample(int &sample_sz)
{
	if (sample_sz > 0)
	{
		--sample_sz;
		GameTime::GetFPS(logFPSComponent); // **passes logFPS function as callback function.
	}
}

// Printe fps (based on InputTester)
void GameComponentAnalytic::PrintFPS()
{
	if (GameTime::getInstance().IsCounting()) // **use IsCounting function to find out if FPS system is running.
		GameTime::GetFPS(printFPSComponent); // **passes printFPS function as callback function.	
}

