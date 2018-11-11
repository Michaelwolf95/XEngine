#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ApplicationManager.h"
//#include "RenderableObject.h"
#include "RenderManager.h"
using namespace std;

int main()
{
	cout << "===== LAUNCHING CECS_491 GAME ENGINE =====" << endl;

	// Init Managers
	ApplicationManager::CreateManager();
	RenderManager::CreateManager();


	// Setup
	//RenderManager::instance->CompileShaders();


	// TEST
	float vertices[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
	RenderableObject* triangle = new RenderableObject(vertices, indices);


	// RENDER LOOP
	// -----------
	while (!ApplicationManager::instance->CheckIfAppShouldClose())
	{
		ApplicationManager::instance->ApplicationStartUpdate();

		RenderManager::instance->Render();
		RenderManager::instance->RenderObject(triangle);
		//// render
		//// ------
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);




		ApplicationManager::instance->ApplicationEndUpdate();
	}

	ApplicationManager::instance->CloseApplication();
	return 0;
}

