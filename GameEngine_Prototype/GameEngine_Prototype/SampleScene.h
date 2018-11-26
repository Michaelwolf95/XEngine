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

#include "CameraComponent.h"
#include "Material.h"
#include "SimpleModelComponent.h"
#include "ExampleRotator.h"
#include "TestMoverComponent.h"

#include "PrimitiveModels.h"

void CreateSampleScene();

// Entry Point called by Main
void CreateScene()
{
	CreateSampleScene();
}

// This is a "Hard-Coded" Scene.
// This process may change in the future to use an editor tool instead.
void CreateSampleScene()
{
	//================================================================
	// 1. CREATE NEW SCENE 
	//================================================================

	// Create sample scene to put our GameObjects in.
	Scene* scene = new Scene("Sample Scene");


	//================================================================
	// 2. CREATE THE CAMERA 
	//================================================================

	// Create an empty GameObject in our scene called "Camera".  
	GameObject* cameraGameObject = scene->CreateGameObject("Camera");

	// Create a CameraComponent that we will view the scene from.
	CameraComponent* cameraComponent = new CameraComponent();

	// Attach the CameraComponent to the GameObject. 
	// The camera will then move along with the GameObject.
	cameraGameObject->AddComponent(cameraComponent);

	// Move the Camera to a good viewing position.
	cameraGameObject->transform->setLocalPosition(glm::vec3(0, 1.5, -5));		// Move back and up.
	cameraGameObject->transform->setLocalRotationEuler(glm::vec3(20, 0, 0));	// Rotate view 20 degrees down.


	//================================================================
	// 3. CREATE THE "CUBE" MODEL 
	//================================================================
	
	// Create an empty GameObject in our scene called "Cube".
	GameObject* cube = scene->CreateGameObject("Cube");

	// Create a shader and material to use to render 3D models.
	Shader* modelShader = new Shader("model.vs", "model.fs");
	Material* boxMaterial = new Material(modelShader);
	
	// Set the texture used for the box material.
	// Note that the file path is relative to the "Assets" folder.
	boxMaterial->LoadTexture("textures/container.jpg");

	// Create the model component using mesh data for a Cube. (See "PrimitiveModels.h")
	SimpleModelComponent* boxModel = new SimpleModelComponent(CUBE_VERTS, 36, 5, CUBE_INDICES, 36);

	boxModel->material = boxMaterial; // Give the model the box material.

	// Attach the boxModel component to the "Cube" GameObject. 
	// This will give the object the ability to render the cube.
	cube->AddComponent(boxModel);


	//================================================================
	// 4. ADD BEHAVIOR TO THE CUBE
	//================================================================

	// Create and add an "ExampleRotator" component to the Cube.
	// This lets you "Rotate" the cube using the arrow keys.
	// You can read more about how it works in by reading "ExampleRotator.cpp"
	ExampleRotator* rotator = new ExampleRotator();
	cube->AddComponent(rotator);
	rotator->rotationSpeed = 90;


	//================================================================
	// 5. CREATE THE FLOOR
	//================================================================

	// Create a "Floor" object and give it a cube shaped model with the same material as the box.
	GameObject* floor = scene->CreateGameObject("Floor");
	SimpleModelComponent* floorModel = new SimpleModelComponent(CUBE_VERTS, 36, 5, CUBE_INDICES, 36);
	floorModel->material = boxMaterial;
	floor->AddComponent(floorModel);

	// Move the floor down and stretch it out to look like a floor.
	floor->transform->setLocalPosition(0, -5, 0);
	floor->transform->setLocalScale(10, 0.5, 10);
	

	//================================================================
	// 6. ACTIVATE SCENE 
	//================================================================

	// Tell the SceneManager to load this scene.
	// This doesn't need to be last, but it's a good practice.
	SceneManager::getInstance().SetActiveScene(scene);


	// Optional: Uncomment this to move your camera around.
	// This component will let you move the camera around using WASD keys, and up and down using the Q&E keys.
	//cameraGameObject->AddComponent(new TestMoverComponent());
}