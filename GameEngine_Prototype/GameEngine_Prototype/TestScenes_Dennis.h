#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "ApplicationManager.h"
#include "AssetManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "FreeLookCameraController.h"
#include "GameObject.h"
#include "Material.h"
#include "SimpleSprite.h"
#include "SimpleModel.h"
#include "SimpleModelComponent.h"
#include "TestMoverComponent.h"
#include "CameraComponent.h"
#include "CameraSwapper.h"
#include "InputTester.h"

void CreateTestScene_DM1();

void RunTestScene_Dennis()
{
	CreateTestScene_DM1();
}

void CreateTestScene_DM1()
{
	Scene* scene = new Scene("DM1-test");
	
	/// Preparing cube model
	// Creating object called "cube"
	GameObject* cube = scene->CreateGameObject("Cube");

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
	modelMaterial->LoadTexture("textures/container.jpg");
	//Input::CreateManager();
	//Input::getInstance().Init()l
	
	SimpleModelComponent* model = new SimpleModelComponent(vertices, 36, 5, indices, sizeof(indices) / sizeof(float), modelMaterial);
	
	model->Setup();
	
	cube->AddComponent(model);
	
	cube->transform->setLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	
	// CAMERA SETUP
	GameObject* camGo = scene->CreateGameObject("Cam");
	CameraComponent* cam = new CameraComponent();
	//FreeLookCameraController* cam = new FreeLookCameraController();

	camGo->AddComponent(cam);
	camGo->transform->Translate(glm::vec3(0.0f, -0.0f, -0.5f)); // y: negative goes up, x: neg goes right, z: neg goes to horizon
	//camGo->transform->Rotate(glm::vec3(15.0f, 0.0f, 0.0f));

	auto inputTester = new InputTester();
	cube->AddComponent(inputTester);
	cube->transform->setLocalPosition(glm::vec3(5.0f, -0.f, -0.0f));
	inputTester->rotationSpeed = 15.0f;
	


	SceneManager::getInstance().SetActiveScene(scene);
} //TODO: delete all these new Objects!!!!
