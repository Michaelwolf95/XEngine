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
//#include "ExampleRotator_James.h"
#include "PrimitiveModels.h"
#include "GameComponentAnalytic.h"
#include "FreeLookCameraController.h"

using namespace std;


void CreateTestScene_GameComponentTest();

/*
float CUBE_VERTS2[] = {
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
unsigned int CUBE_INDICES2[] = {
	0, 1, 2,	0, 2, 3,      // front
	4, 5, 6,	4, 6, 7,      // back
	8, 9, 10,	8, 10, 11,    // top
	12, 13, 14, 12, 14, 15,   // bottom
	16, 17, 18, 16, 18, 19,   // right
	20, 21, 22, 20, 22, 23,   // left
};
*/
void RunTestScene_Tam()
{
	CreateTestScene_GameComponentTest();
}

void CreateTestScene_GameComponentTest()
{
	Scene* scene = new Scene("Testing 3D Model");

	
	// make 3D model
	
	
	//Shader* modelShader = new Shader("model.vs", "model.fs");
	
	//Shader* modelShaderCUBE = new Shader("model.vs", "model.fs");
	//Material* modelMaterialCUBE = new Material(modelShaderCUBE);
	//modelMaterialCUBE->LoadTexture("textures/container.jpg");

	//GameObject* cube = scene->CreateGameObject("Cube");
	//SimpleModelComponent* model = new SimpleModelComponent(CUBE_VERTS, 36, 5, CUBE_INDICES, 36, modelMaterialCUBE);// here's the culprit
	//cube->AddComponent(model);
	//cube->transform->setLocalPosition(1, 0, 0);


	Shader* modelShader = new Shader("3Dmodel.vs", "3Dmodel.fs");
	Material* modelMaterial = new Material(modelShader);
	MeshRenderer* modelNano = new MeshRenderer("C:/Users/Simba/Documents/CECS_491_GameEngine_Prototype/GameEngine_Prototype/Assets/3Dmodel/nanosuit/nanosuit.obj", modelMaterial);
	//MeshRenderer* modelNano = new MeshRenderer("C:/Users/Simba/Documents/CECS_491_GameEngine_Prototype/GameEngine_Prototype/Assets/3Dmodel/Crate/Crate1.obj", modelMaterial);
	GameObject* modelMan = scene->CreateGameObject("ModelMan");
	modelMan->AddComponent(modelNano);
	modelMan->transform->setLocalPosition(0, 0, 0);
	
	//C:\Users\Simba\Documents\CECS_491_GameEngine_Prototype\GameEngine_Prototype\Assets\3Dmodel\nanosuit
//MeshRenderer* modelNano = new MeshRenderer("C:/Users/Simba/Documents/CECS_491_GameEngine_Prototype/GameEngine_Prototype/Assets/3Dmodel/h66xqn7m98u8-Rubics_cube/Rubics_cube/cube.obj", modelMaterial);

	TestMoverComponent* move = new TestMoverComponent();
	modelMan->AddComponent(move);

	// make camera
	GameObject* camGo = scene->CreateGameObject("Camera");
	CameraComponent* camera = new CameraComponent();
	camGo->AddComponent(camera);
	//FreeLookCameraController* moving = new  FreeLookCameraController();
	//camGo->AddComponent(moving);

	camGo->transform->setLocalPosition(glm::vec3(0, 1, -5));
	camGo->transform->setLocalRotationEuler(glm::vec3(20, 0, 0));
	

	SceneManager::getInstance().SetActiveScene(scene);
}