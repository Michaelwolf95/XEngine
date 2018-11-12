#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ApplicationManager.h"
#include "RenderManager.h"
//#include "RenderableObject.h"
using namespace std;

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

	// RENDER LOOP
	// -----------
	while (!ApplicationManager::instance->CheckIfAppShouldClose())
	{
		ApplicationManager::instance->ApplicationStartUpdate();


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
	RenderableObject* shape2 = new RenderableObject(v2, 12, i2, 6);
}