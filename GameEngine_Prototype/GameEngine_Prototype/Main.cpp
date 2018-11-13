#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_DEFINE  
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "AssetManager.h"
#include "ApplicationManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "Time.h"

#include "TestScenes.h"
using namespace std;


// ENTRY POINT
int main()
{
	cout << "===== LAUNCHING CECS_491 GAME ENGINE =====" << endl;

	// Init Managers
	ApplicationManager::CreateManager();
	RenderManager::CreateManager();
	SceneManager::CreateManager();
	Time::CreateManager();

	// Test
	RunTestScene();

	// RENDER LOOP
	// -----------
	while (!ApplicationManager::instance->CheckIfAppShouldClose())
	{
		ApplicationManager::instance->ApplicationStartUpdate();
		Time::instance->UpdateTime();

		// Do Game Logic here
		SceneManager::instance->UpdateActiveScene();

		RenderManager::instance->Render();

		ApplicationManager::instance->ApplicationEndUpdate();
	}

	ApplicationManager::instance->CloseApplication();
	return 0;
}
