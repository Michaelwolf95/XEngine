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
#include "FreeLookCameraController.h"
#include "SimpleRotator.h"
#include "TransformTester.h"
#include "SceneEditor.h"
#include "EditorCamera.h"
using namespace std;

#include "PrimitiveModels.h"

//void ChooseTestScene();
//// Not a "scene" per-say. Just tests.
//void CreateTestScene1();
//void CreateTestScene2();
//void CreateTestScene3();
//void CreateTestScene4();
//// Actual Scenes
////void CreateTestScene5();
////void CreateTestScene6();
////void CreateTestScene7();
////void CreateTestScene8();
//void CreateTestScene_EXAMPLE();

void CreateTestScene_Serialized();


void RunTestScene()
{
	//CreateTestScene_EXAMPLE();
	//CreateTestScene7();
	CreateTestScene_Serialized();
}


void CreateTestScene_Serialized()
{
	//std::cout << "Serial Scene" << std::endl;

	//Scene* scene = nullptr;
	//Scene sc = *new Scene("Serial_Scene");
	Scene_ptr scene(new Scene("Serial_Scene"));
	bool exists = SceneManager::getInstance().LoadSceneFromFileByName(*scene, "Serial_Scene");
	if (exists)
	{
		std::cout << "Loading Scene from File!" << std::endl;
		//scene = &sc;
		// Activate Scene
		SceneManager::getInstance().SetActiveScene(scene);
		return;
	}
	std::cout << "Generating new scene!" << std::endl;
	scene = std::make_shared<Scene>(Scene("Serial_Scene"));

	// Create Scene
	GameObject_ptr editorGo = scene->CreateGameObject("SceneEditor");
	//editorGo->AddComponent(new SceneEditor());
	editorGo->AddComponent(std::make_shared<EditorCamera>(EditorCamera()));

	GameObject_ptr go = scene->CreateGameObject("Cube");
	// Create Box Material
	//Shader* modelShader = new Shader("model.vs", "model.fs");
	Material* modelMaterial = new Material("model.vs", "model.fs");
	modelMaterial->LoadTexture("textures/container.jpg");
	std::shared_ptr < SimpleModelComponent> testModel(new SimpleModelComponent(CUBE_VERTS, 36, 5,
		CUBE_INDICES, sizeof(CUBE_INDICES) / sizeof(unsigned int), modelMaterial));
	testModel->Setup();
	go->AddComponent(testModel);

	go->AddComponent(std::make_shared<TestMoverComponent>(TestMoverComponent()));

	//// Create Floor
	//GameObject* go2 = scene->CreateGameObject("Floor");
	//SimpleModelComponent* testModel2 = new SimpleModelComponent(CUBE_VERTS, 36, 5,
	//	CUBE_INDICES, sizeof(CUBE_INDICES) / sizeof(unsigned int), modelMaterial);
	//testModel2->Setup();
	////go2->AddComponent(testModel2);
	//go2->transform->Translate(glm::vec3(0.0f, -2.5f, -10.0f));
	//go2->transform->Scale(glm::vec3(10.0f, 0.5f, 10.0f));

	// SAVE SCENE
	SceneManager::getInstance().SaveSceneToFile(*scene);

	// Activate Scene
	SceneManager::getInstance().SetActiveScene(scene);
}

/*
void CreateTestScene_EXAMPLE()
{
	Scene* scene = nullptr;
	Scene sc;// ("EXAMPLE SCENE");
	bool exists = SceneManager::getInstance().LoadSceneFromFile(sc, "../Scenes/EXAMPLE SCENE.txt");
	if (exists)
	{
		std::cout << "Loading Scene from File!" << std::endl;
		scene = &sc;
		// Activate Scene
		SceneManager::getInstance().SetActiveScene(scene);
		return;
	}
	std::cout << "Generating new scene!" << std::endl;
	scene = new Scene("EXAMPLE SCENE");

	GameObject_ptr cube = scene->CreateGameObject("Cube");
	// Create Cube Model
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

	std::shared_ptr <SimpleModelComponent> model(new SimpleModelComponent(vertices, 36, 5, indices, 36, modelMaterial));
	cube->AddComponent(model);
	
	cube->transform->setLocalPosition(glm::vec3(0, 0, 0));

	// Create Camera GameObject
	GameObject_ptr camGo = scene->CreateGameObject("Camera");
	std::shared_ptr <CameraComponent> camera(new CameraComponent());
	camGo->AddComponent(camera);

	camGo->transform->setLocalPosition(glm::vec3(0, 2, -5));
	camGo->transform->setLocalRotationEuler(glm::vec3(20, 0, 0));

	//auto rotator = new TransformTester();
	//cube->AddComponent(rotator);
	//rotator->rotationSpeed = 90;

	// SAVE SCENE
	SceneManager::getInstance().SaveSceneToFile(*scene);

	SceneManager::getInstance().SetActiveScene(scene);
}


void ChooseTestScene()
{
	cout << "Choose Scene to Load [1-5]:" << endl;

	int choice = -1;
	while (choice < 0)
	{
		cin >> choice;
	}
	switch (choice)
	{
	default:
	case 1:
		CreateTestScene1();
		break;
	case 2:
		CreateTestScene2();
		break;
	case 3:
		CreateTestScene3();
		break;
	case 4:
		CreateTestScene4();
		break;
	case 5:
		//CreateTestScene5();
		break;
	}
}

void CreateTestScene1()
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
	SimpleSprite* testSprite = new SimpleSprite(
		vertices, 4, 8,
		indices, 6,
		spriteMaterial);
	testSprite->Setup();
	spriteMaterial->LoadTexture("textures/container.jpg");

}

void CreateTestScene3()
{
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

void CreateTestScene4()
{
	Shader* modelShader = new Shader("model.vs", "model.fs");
	Material* modelMaterial = new Material(modelShader);
	SimpleModel* testModel = new SimpleModel(CUBE_VERTS, 36, 5,
		CUBE_INDICES, sizeof(CUBE_INDICES) / sizeof(unsigned int), modelMaterial);
	testModel->Setup();
	modelMaterial->LoadTexture("textures/container.jpg");

}
*/
/*
void CreateTestScene5()
{
	// Create Box Material
	Shader* modelShader = new Shader("model.vs", "model.fs");
	Material* modelMaterial = new Material(modelShader);
	modelMaterial->LoadTexture("textures/container.jpg");

	SimpleModelComponent* testModel = new SimpleModelComponent(CUBE_VERTS, 36, 5,
		CUBE_INDICES, sizeof(CUBE_INDICES) / sizeof(unsigned int), modelMaterial);
	testModel->Setup();

	// Create Scene
	Scene* scene = new Scene("Test Scene 5");

	GameObject_ptr go = scene->CreateGameObject("Cube");
	go->AddComponent(testModel);
	go->AddComponent(new TestMoverComponent());

	// Create Floor
	GameObject_ptr go2 = scene->CreateGameObject("Floor");
	SimpleModelComponent* testModel2 = new SimpleModelComponent(CUBE_VERTS, 36, 5,
		CUBE_INDICES, sizeof(CUBE_INDICES) / sizeof(unsigned int), modelMaterial);
	testModel2->Setup();
	go2->AddComponent(testModel2);
	go2->transform->Translate(glm::vec3(0.0f, -2.5f, -10.0f));
	go2->transform->Scale(glm::vec3(10.0f, 0.5f, 10.0f));

	// Activate Scene
	SceneManager::getInstance().SetActiveScene(scene);
}

void CreateTestScene6()
{
	// Create Box Material
	Shader* modelShader = new Shader("model.vs", "model.fs");
	Material* modelMaterial = new Material(modelShader);
	modelMaterial->LoadTexture("textures/container.jpg");

	SimpleModelComponent* testModel = new SimpleModelComponent(CUBE_VERTS, 36, 5,
		CUBE_INDICES, sizeof(CUBE_INDICES) / sizeof(unsigned int), modelMaterial);
	testModel->Setup();


	// Create Scene
	Scene* scene = new Scene("Test Scene 6: Cameras");

	GameObject_ptr go = scene->CreateGameObject("Cube");
	go->AddComponent(testModel);
	//go->AddComponent(new TestMoverComponent());

	// Create Floor
	GameObject_ptr go2 = scene->CreateGameObject("Floor");
	SimpleModelComponent* testModel2 = new SimpleModelComponent(CUBE_VERTS, 36, 5,
		CUBE_INDICES, sizeof(CUBE_INDICES) / sizeof(unsigned int), modelMaterial);
	testModel2->Setup();
	go2->AddComponent(testModel2);
	go2->transform->Translate(glm::vec3(0.0f, -2.5f, 0.0f));
	go2->transform->Scale(glm::vec3(10.0f, 0.5f, 10.0f));

	// CAMERA SETUP

	GameObject_ptr camGo1 = scene->CreateGameObject("Cam1");
	CameraComponent* cam1 = new CameraComponent();
	camGo1->AddComponent(cam1);
	camGo1->transform->Translate(glm::vec3(1.0f, -1.0f, -7.0f));
	camGo1->transform->Rotate(glm::vec3(0.0f, 20.0f, 0.0f));
	camGo1->transform->Rotate(glm::vec3(10.0f, 0.0f, 0.0f));

	GameObject_ptr camGo2 = scene->CreateGameObject("Cam2");
	CameraComponent* cam2 = new CameraComponent();
	camGo2->AddComponent(cam2);
	camGo2->transform->Translate(glm::vec3(-1.0f, -1.0f, -7.0f));
	camGo2->transform->Rotate(glm::vec3(0.0f, -20.0f, 0.0f));
	camGo2->transform->Rotate(glm::vec3(10.0f, 0.0f, 0.0f));

	GameObject_ptr camGo3 = scene->CreateGameObject("Cam3 (Moving)");
	std::shared_ptr<CameraComponent> cam3(new CameraComponent());
	camGo3->AddComponent(cam3);
	camGo3->transform->Rotate(glm::vec3(10.0f, 0.0f, 0.0f));
	camGo3->AddComponent(std::make_shared<TestMoverComponent>(new TestMoverComponent()));

	GameObject_ptr swapperGo = scene->CreateGameObject("Swapper");
	CameraSwapper* swapper = new CameraSwapper();
	swapperGo->AddComponent(swapper);
	swapper->cam1 = cam1;
	swapper->cam2 = cam2;
	swapper->cam3 = cam3;


	// Activate Scene
	SceneManager::getInstance().SetActiveScene(scene);
}

void CreateTestScene7()
{
	// Create Box Material
	Shader* modelShader = new Shader("diffuse.vs", "diffuse.fs");
	Material* modelMaterial = new Material(modelShader, true);
	modelMaterial->LoadTexture("textures/container.jpg");

	Shader* unlitShader = new Shader("model.vs", "model.fs");
	Material* unlitMaterial = new Material(unlitShader);
	unlitMaterial->Color = glm::vec4(1.0, 1.0, 1.0, 1.0);
	//unlitMaterial->LoadTexture("textures/container.jpg");

	SimpleModelComponent* testModel = new SimpleModelComponent(CUBE_VERTS, 36, 5,
		CUBE_INDICES, sizeof(CUBE_INDICES) / sizeof(unsigned int), modelMaterial);
	testModel->Setup();


	// Create Scene
	Scene* scene = new Scene("Test Scene 7: Lighting");

	GameObject_ptr go = scene->CreateGameObject("Cube");
	go->AddComponent(testModel);
	//go->AddComponent(new TestMoverComponent());

	GameObject_ptr lightGo = scene->CreateGameObject("Light");
	lightGo->AddComponent(new LightComponent());
	lightGo->transform->Translate(glm::vec3(0.0f, 1.5f, 0.0f));
	lightGo->transform->Scale(glm::vec3(0.2f));
	lightGo->AddComponent(new TestMoverComponent());
	SimpleModelComponent* lamp = new SimpleModelComponent(CUBE_VERTS, 36, 5,
		CUBE_INDICES, sizeof(CUBE_INDICES) / sizeof(unsigned int), unlitMaterial);
	lamp->Setup();
	lightGo->AddComponent(lamp);


	// CAMERA SETUP
	GameObject_ptr camGo = scene->CreateGameObject("Cam");
	CameraComponent* cam3 = new CameraComponent();
	camGo->AddComponent(cam3);
	camGo->transform->Translate(glm::vec3(0.0f, -1.0f, -4.5f));
	camGo->transform->Rotate(glm::vec3(-15.0f, 0.0f, 0.0f));
	//camGo->AddComponent(new TestMoverComponent());


	// Activate Scene
	SceneManager::getInstance().SetActiveScene(scene);
}

void CreateTestScene8()
{
	// Create Box Material
	Shader* modelShader = new Shader("model.vs", "model.fs");
	Material* modelMaterial = new Material(modelShader);
	modelMaterial->LoadTexture("textures/container.jpg");
	modelMaterial->Color = vec4(1, 1, 1, 1);

	// Create Scene
	Scene* scene = new Scene("Test Scene 8: Transform Tests");

	// Create Floor
	GameObject_ptr go2 = scene->CreateGameObject("Floor");
	SimpleModelComponent* floorModel2 = new SimpleModelComponent(
		CUBE_VERTS, 36, 5,
		CUBE_INDICES, sizeof(CUBE_INDICES) / sizeof(unsigned int),
		modelMaterial);
	floorModel2->Setup();
	go2->AddComponent(floorModel2);
	go2->transform->Translate(glm::vec3(0.0f, -5.0f, 0.0f));
	go2->transform->Scale(glm::vec3(10.0f, 0.5f, 10.0f));

	GameObject_ptr go = scene->CreateGameObject("Cube");
	SimpleModelComponent* cubeModel = new SimpleModelComponent(CUBE_VERTS, 36, 5,
		CUBE_INDICES, sizeof(CUBE_INDICES) / sizeof(unsigned int), modelMaterial);
	cubeModel->Setup();
	go->AddComponent(cubeModel);

	go->transform->setLocalPosition(vec3(1, 0, 1));
	go->transform->setLocalScale(vec3(3, 1, 2));
	go->transform->setLocalRotationEuler(vec3(35, 0, 0));
	//go->transform->setLocalRotationEuler(vec3(15, 45, -20));

	glm::vec3 rot = go->transform->getLocalRotationEuler();
	std::cout << "Rot:  (" << rot.x << ", " << rot.y << ", " << rot.z << ")" << std::endl;
	glm::vec3 sc = go->transform->getLocalScale();
	std::cout << "Scale:(" << sc.x << ", " << sc.y << ", " << sc.z << ")" << std::endl;
	EngineDebug::PrintMatrix(go->transform->getTranslationMatrix());
	EngineDebug::PrintMatrix(go->transform->getRotationMatrix());
	EngineDebug::PrintMatrix(go->transform->getScaleMatrix());

	auto rotator = new SimpleRotator();
	go->AddComponent(rotator);

	// CAMERA SETUP
	GameObject_ptr camGo = scene->CreateGameObject("Cam");
	CameraComponent* cam3 = new CameraComponent();
	camGo->AddComponent(cam3);
	camGo->transform->Translate(glm::vec3(0.0f, -1.0f, -8.0f));
	camGo->transform->Rotate(glm::vec3(15.0f, 0.0f, 0.0f));
	//camGo->AddComponent(new TestMoverComponent());
	camGo->AddComponent(new FreeLookCameraController());

	// Activate Scene
	SceneManager::getInstance().SetActiveScene(scene);
}
*/