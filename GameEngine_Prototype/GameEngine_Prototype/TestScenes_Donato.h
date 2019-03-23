#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "ApplicationManager.h"
#include "AssetManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "MeshRenderer.h"
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
#include "PrimitiveModels.h"
#include "GameComponentAnalytic.h"
#include "FreeLookCameraController.h"
#include "Text.h"

void CreateTestScene_Donato()
{
	Scene_ptr scene(new Scene("Loading 3d models"));

	Text t1("IHATCS__.ttf", 40);
	t1.RenderText("HISDFBDSJFBSDFJDSBFSD", 0.0f, 0.0f, 2.0f, glm::vec3(0.5, 0.8f, 0.2f));

	Material* modelMaterial = new Material("ModelManTest", "3Dmodel.vs", "3Dmodel.fs");
	std::shared_ptr<MeshRenderer> modelNano(new MeshRenderer("3Dmodel/Crate/Crate1.obj", modelMaterial));
	GameObject_ptr modelMan = scene->CreateGameObject("ModelMan");

	modelMan->AddComponent(modelNano);
	modelMan->transform->setLocalPosition(0, 0, 0);

	std::shared_ptr<TestMoverComponent> move(new TestMoverComponent());
	modelMan->AddComponent(move);

	modelMan->transform->setLocalScale(0.5f, 0.5f, 0.5f);

	// make camera
	GameObject_ptr camGo = scene->CreateGameObject("Camera");
	std::shared_ptr<CameraComponent> camera(new CameraComponent());
	camGo->AddComponent(camera);
	std::shared_ptr<FreeLookCameraController> moving(new FreeLookCameraController());
	camGo->AddComponent(moving);

	camGo->transform->setLocalPosition(glm::vec3(0, 1, -5));

	SceneManager::getInstance().SetActiveScene(scene);
}

void RunTestScene_Donato()
{
	CreateTestScene_Donato();
}