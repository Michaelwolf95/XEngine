#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ApplicationManager.h"
using namespace std;

int main()
{
	cout << "===== LAUNCHING CECS_491 GAME ENGINE =====" << endl;

	// Init Managers
	ApplicationManager::CreateManager();


	cout << "Opening Window" << endl;

	// RENDER LOOP
	// -----------
	while (!ApplicationManager::instance->CheckIfAppShouldClose())
	{
		ApplicationManager::instance->ApplicationStartUpdate();


		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);




		ApplicationManager::instance->ApplicationEndUpdate();
	}

	ApplicationManager::instance->CloseApplication();
	return 0;
}

