#pragma once
#include "AssetManager.h"
#include "ApplicationManager.h"
#include "RenderManager.h"
#include "GameObject.h"
#include "Material.h"
#include "SimpleSprite.h"
#include "SimpleModel.h"
#include "SimpleModelComponent.h"
#include "TestMoverComponent.h"
using namespace std;

// Not a "scene" per-say - until the scene manager is implemented
void CreateTestScene1();
void CreateTestScene2();
void CreateTestScene3();
void CreateTestScene4();
void CreateTestScene5();
void ChooseTestScene();

void RunTestScene()
{
	CreateTestScene5();
}

void ChooseTestScene()
{
	cout << "Choose Scene to Load [1-5]:" << endl;

	int choice = -1;
	while (choice < 0)
	{
		cin >> choice;
	}
	switch (choice)
	{
	default:
	case 1:
		CreateTestScene1();
		break;
	case 2:
		CreateTestScene2();
		break;
	case 3:
		CreateTestScene3();
		break;
	case 4:
		CreateTestScene4();
		break;
	case 5:
		//CreateTestScene5();
		break;
	}
}

void CreateTestScene1()
{
	// TEST
	float v1[] = {
		-0.1f,  0.9f, 0.0f,  // top right
		-0.1f, 0.1f, 0.0f,  // bottom right
		-0.9f, 0.1f, 0.0f,  // bottom left
		-0.9f,  0.9f, 0.0f   // top left 
	};
	unsigned int i1[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
	RenderableObject* shape = new RenderableObject(v1, 4, 3, i1, 6);
	shape->Setup();

	float v2[] = {
		0.9f,  0.9f, 0.0f,  // top right
		0.9f, 0.1f, 0.0f,  // bottom right
		0.1f, 0.1f, 0.0f,  // bottom left
		0.1f,  0.9f, 0.0f   // top left 
	};
	unsigned int i2[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
	Shader* defaultShader = new Shader("default.vs", "default.fs");
	Material* spriteMat1 = new Material(defaultShader);
	SimpleSprite* sprite = new SimpleSprite(v2, 4, 3, i2, 6, spriteMat1);
	sprite->Setup();

	spriteMat1->Color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	float v3[] = {
		-0.1f,  -0.1f, 0.0f,  // top right
		-0.1f, -0.9f, 0.0f,  // bottom right
		-0.9f, -0.9f, 0.0f,  // bottom left
		-0.9f,  -0.1f, 0.0f   // top left 
	};
	unsigned int i3[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
	Material* spriteMat2 = new Material(defaultShader);
	SimpleSprite* sprite2 = new SimpleSprite(v3, 4, 3, i3, 6, spriteMat2);
	sprite2->Setup();
	spriteMat2->Color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // Green

	float v4[] = {
		// positions          // colors           // texture coords
		0.9f,  -0.1f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		0.9f, -0.9f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		0.1f, -0.9f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		0.1f,  -0.1f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int i4[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	Shader* spriteShader = new Shader("sprite.vs", "sprite.fs");
	Material* spriteMaterial = new Material(spriteShader);
	spriteMaterial->LoadTexture("textures/container.jpg");

	SimpleSprite* sprite3 = new SimpleSprite(
		v4, 4, 8,
		i4, 6,
		spriteMaterial);
	sprite3->Setup();
	//sprite3->LoadTexture("textures/container.jpg");

}

void CreateTestScene2()
{
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	Shader* spriteShader = new Shader("sprite.vs", "sprite.fs");
	Material* spriteMaterial = new Material(spriteShader);
	SimpleSprite* testSprite = new SimpleSprite(
		vertices, 4, 8,
		indices, 6,
		spriteMaterial);
	testSprite->Setup();
	spriteMaterial->LoadTexture("textures/container.jpg");

}

void CreateTestScene3()
{
	//GameObject* go = new GameObject("");
	float vertices[] = {
		// positions          // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	Shader* modelShader = new Shader("model.vs", "model.fs");
	Material* modelMaterial = new Material(modelShader);
	SimpleModel* testModel = new SimpleModel(
		vertices, 4, 5,
		indices, 6,
		modelMaterial);
	testModel->Setup();
	modelMaterial->LoadTexture("textures/container.jpg");
	//modelMaterial->Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // It can change color!
}

void CreateTestScene4()
{
	// Cube
	float vertices[] = {
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
	unsigned int indices[] = {
		0, 1, 2,	0, 2, 3,      // front
		4, 5, 6,	4, 6, 7,      // back
		8, 9, 10,	8, 10, 11,    // top
		12, 13, 14, 12, 14, 15,   // bottom
		16, 17, 18, 16, 18, 19,   // right
		20, 21, 22, 20, 22, 23,   // left
	};
	Shader* modelShader = new Shader("model.vs", "model.fs");
	Material* modelMaterial = new Material(modelShader);
	SimpleModel* testModel = new SimpleModel(
		vertices, 36, 5,
		indices, sizeof(indices) / sizeof(unsigned int),
		modelMaterial);
	testModel->Setup();
	modelMaterial->LoadTexture("textures/container.jpg");

}

void CreateTestScene5()
{
	// Cube
	float vertices[] = {
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
	unsigned int indices[] = {
		0, 1, 2,	0, 2, 3,      // front
		4, 5, 6,	4, 6, 7,      // back
		8, 9, 10,	8, 10, 11,    // top
		12, 13, 14, 12, 14, 15,   // bottom
		16, 17, 18, 16, 18, 19,   // right
		20, 21, 22, 20, 22, 23,   // left
	};
	Shader* modelShader = new Shader("model.vs", "model.fs");
	Material* modelMaterial = new Material(modelShader);
	SimpleModelComponent* testModel = new SimpleModelComponent(
		vertices, 36, 5,
		indices, sizeof(indices) / sizeof(unsigned int),
		modelMaterial);
	testModel->Setup();
	modelMaterial->LoadTexture("textures/container.jpg");


	Scene* scene = new Scene("Test Scene 5");
	GameObject* go = scene->CreateGameObject("Cube");
	go->AddComponent(testModel);
	go->AddComponent(new TestMoverComponent());

	SceneManager::instance->SetActiveScene(scene);
}