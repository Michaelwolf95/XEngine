#include "GameObjectAnalytic.h"
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
#include "ExampleRotator_James.h"
//#include "PrimitiveModels.h"

GameObjectAnalytic::GameObjectAnalytic(){}

GameObjectAnalytic::~GameObjectAnalytic(){}

void GameObjectAnalytic::Start()
{
	srand(time(NULL));


	Scene_ptr scene = SceneManager::getInstance().GetActiveScene();
	//Shader* modelShader = new Shader("model.vs", "model.fs");
	Material* modelMaterial = new Material("Simple Model","model.vs", "model.fs");

	// DEPRECATED
	//modelMaterial->LoadTexture("textures/container.jpg"); //change model here if needed for different test

	int range = 10;
	//loop to create more gameObjects
	for (int i = 1; i < 50; i++)
	{
		GameObject_ptr c = scene->CreateGameObject("Cube");
		std::shared_ptr<SimpleModelComponent> m(new SimpleModelComponent("Cube", CUBE_VERTS, 36, 5, CUBE_INDICES, 36, modelMaterial));
		c->AddComponent(m);
		c->transform->setLocalPosition((rand() % range) + 1, (rand() % range) + 1, (rand() % range) + 1);
		std::shared_ptr<ExampleRotator_James> rotator(new ExampleRotator_James());
		c->AddComponent(rotator); //rotating all the GameObjects
	}
}

void GameObjectAnalytic::Update()
{
	// Code that happens every frame. 
}
