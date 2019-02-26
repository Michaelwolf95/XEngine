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

	float cube_verts[] = {
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

	unsigned int cube_indices[] = {
	0, 1, 2,	0, 2, 3,      // front
	4, 5, 6,	4, 6, 7,      // back
	8, 9, 10,	8, 10, 11,    // top
	12, 13, 14, 12, 14, 15,   // bottom
	16, 17, 18, 16, 18, 19,   // right
	20, 21, 22, 20, 22, 23,   // left
	};

	Scene* scene = SceneManager::getInstance().GetActiveScene();
	Shader* modelShader = new Shader("model.vs", "model.fs");
	Material* modelMaterial = new Material(modelShader);
	modelMaterial->LoadTexture("textures/container.jpg"); //change model here if needed for different test

	int range = 10;
	//loop to create more gameObjects
	for (int i = 1; i < 50; i++)
	{
		GameObject* c = scene->CreateGameObject("Cube");
		SimpleModelComponent* m = new SimpleModelComponent(cube_verts, 36, 5, cube_indices, 36, modelMaterial);
		c->AddComponent(m);
		c->transform->setLocalPosition((rand() % range) + 1, (rand() % range) + 1, (rand() % range) + 1);
		c->AddComponent(new ExampleRotator_James()); //rotating all the GameObjects
	}
}

void GameObjectAnalytic::Update()
{
	// Code that happens every frame. 
}
