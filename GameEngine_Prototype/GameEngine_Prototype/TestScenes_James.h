#pragma once
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
#include "TestMoverComponent.h"
#include "CameraComponent.h"
#include "CameraSwapper.h"
#include "SimpleRotator.h"
#include "ExampleRotator_James.h"
#include "PrimitiveModels.h"
#include "GameObjectAnalytic.h"
#include "FreeLookCameraController.h"
using namespace std;

void CreateTestSceneJames1();

void RunTestScene_James()
{
	CreateTestSceneJames1();
}

void CreateTestSceneJames1() {
	Scene* scene = new Scene("SCENE1");

	Shader* modelShader = new Shader("model.vs", "model.fs");
	Material* modelMaterial = new Material(modelShader);
	modelMaterial->LoadTexture("textures/container.jpg");

	GameObject* cube = scene->CreateGameObject("Cube");
	SimpleModelComponent* model = new SimpleModelComponent(CUBE_VERTS, 36, 5, CUBE_INDICES, 36, modelMaterial);
	cube->AddComponent(model);
	cube->transform->setLocalPosition(0, 0, 0);

	// Create Camera GameObject
	GameObject* camGo = scene->CreateGameObject("Camera");
	CameraComponent* camera = new CameraComponent();
	camGo->AddComponent(camera);
	//make camera move
	FreeLookCameraController* moving = new  FreeLookCameraController();
	camGo->AddComponent(moving);

	camGo->transform->setLocalPosition(glm::vec3(0, 1, -5)); //setting position of the camera
	camGo->transform->setLocalRotationEuler(glm::vec3(20, 0, 0)); //setting position of the camera

	auto rotator = new ExampleRotator_James();
	cube->AddComponent(new ExampleRotator_James());
	rotator->rotationSpeed = 10;

	//testing by creating more GameObjects------------------------------------------
	GameObjectAnalytic* testComponent = new GameObjectAnalytic();
	cube->AddComponent(testComponent);

	SceneManager::getInstance().SetActiveScene(scene);
}