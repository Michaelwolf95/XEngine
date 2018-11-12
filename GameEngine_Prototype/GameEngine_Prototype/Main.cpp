#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ApplicationManager.h"
#include "RenderManager.h"
#include "SimpleSprite.h"

using namespace std;


SimpleSprite* sprite;

// Not a "scene" per-say.
void CreateTestScene();

int main()
{
	cout << "===== LAUNCHING CECS_491 GAME ENGINE =====" << endl;

	// Init Managers
	ApplicationManager::CreateManager();
	RenderManager::CreateManager();

	// Test
	CreateTestScene();

	if (ApplicationManager::APP_WINDOW == NULL)
	{
		std::cout << "App Window is Null! (2)" << std::endl;
	}

	// RENDER LOOP
	// -----------
	while (!ApplicationManager::instance->CheckIfAppShouldClose())
	{
		ApplicationManager::instance->ApplicationStartUpdate();

		// Do Game Logic here

		if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_E) == GLFW_PRESS)
		{
			//glfwSetWindowShouldClose(window, true);
			sprite->enabled = !sprite->enabled;
		}


		RenderManager::instance->Render();

		ApplicationManager::instance->ApplicationEndUpdate();
	}

	ApplicationManager::instance->CloseApplication();
	return 0;
}

void CreateTestScene()
{
	// TEST
	float vertices[] = {
		-0.75f,  0.5f, 0.0f,  // top right
		-0.75f, -0.5f, 0.0f,  // bottom right
		-0.1f, -0.5f, 0.0f,  // bottom left
		-0.1f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
	RenderableObject* shape = new RenderableObject(vertices, 12, indices, 6);

	float v2[] = {
		0.75f,  0.5f, 0.0f,  // top right
		0.75f, -0.5f, 0.0f,  // bottom right
		0.1f, -0.5f, 0.0f,  // bottom left
		0.1f,  0.5f, 0.0f   // top left 
	};
	unsigned int i2[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
	Shader* spriteShader = new Shader("default.vs", "default.fs");
	sprite = new SimpleSprite(v2, 12, i2, 6, spriteShader);

	sprite->Color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
}