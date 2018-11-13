#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_DEFINE  
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "AssetManager.h"
#include "ApplicationManager.h"
#include "RenderManager.h"
#include "GameObject.h"
#include "Material.h"

#include "SimpleSprite.h"
#include "SimpleModel.h"
using namespace std;


SimpleSprite* testSprite;

// Not a "scene" per-say.
void CreateTestScene();
void CreateTestScene2();
void CreateTestScene3();

// ENTRY POINT
int main()
{
	cout << "===== LAUNCHING CECS_491 GAME ENGINE =====" << endl;

	// Init Managers
	ApplicationManager::CreateManager();
	RenderManager::CreateManager();

	// Test
	//CreateTestScene();
	//CreateTestScene2();
	CreateTestScene3();

	// RENDER LOOP
	// -----------
	while (!ApplicationManager::instance->CheckIfAppShouldClose())
	{
		ApplicationManager::instance->ApplicationStartUpdate();

		// Do Game Logic here


		RenderManager::instance->Render();

		ApplicationManager::instance->ApplicationEndUpdate();
	}

	ApplicationManager::instance->CloseApplication();
	return 0;
}

void CreateTestScene()
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
	testSprite = new SimpleSprite(
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