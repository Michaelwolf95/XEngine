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
#include "input.h"
#include "AudioComponent.h"
using namespace std;

void CreateTestSceneJames1();

void RunTestScene_James()
{
	CreateTestSceneJames1();
}

void CreateTestSceneJames1() {
	Scene_ptr scene(new Scene("SCENE1"));

	Shader* modelShader = new Shader("model.vs", "model.fs");
	Material* modelMaterial = new Material(modelShader);
	modelMaterial->LoadTexture("textures/container.jpg");

	GameObject_ptr cube = scene->CreateGameObject("Cube");
	std::shared_ptr<SimpleModelComponent> model(new SimpleModelComponent(CUBE_VERTS, 36, 5, CUBE_INDICES, 36, modelMaterial));
	cube->AddComponent(model);
	cube->transform->setLocalPosition(0, 0, 0);



	//change the moving ovject to the cube and set the camrea to the postion 000


	// Create Camera GameObject
	GameObject_ptr camGo = scene->CreateGameObject("Camera");
	std::shared_ptr <CameraComponent> camera(new CameraComponent());
	camGo->AddComponent(camera);
	//make camera move
	std::shared_ptr <FreeLookCameraController> moving(new FreeLookCameraController());
	//camGo->AddComponent(moving);------------------------------------------------

	cube->AddComponent(moving);

	camGo->transform->setLocalPosition(glm::vec3(0, 0, 0)); //setting position of the camera
	camGo->transform->setLocalRotationEuler(glm::vec3(0, 0, 0)); //setting position of the camera


	//disabling mouse cursor
	Input::getInstance().ShowCursor(false);
	//toggling the cursor
	//bool toggleCursor();


	//rotating----------------------------------------------------------------------
	//auto rotator = new ExampleRotator_James();
	//cube->AddComponent(new ExampleRotator_James());
	//rotator->rotationSpeed = 10;

	//testing by creating more GameObjects------------------------------------------
	//GameObjectAnalytic* testComponent = new GameObjectAnalytic();
	//cube->AddComponent(testComponent);



	//void Set3dListenerAndOrientation(const Vector3& vPos = Vector3{ 0, 0, 0 }, float fVolumedB = 0.0f);
	//void PlaySound(const string& strSoundName, const Vector3& vPos = Vector3{ 0, 0, 0 }, float fVolumedB = 0.0f)


	//Audio
	glm::vec3 loc = cube->transform->getPosition();
	Vector3 location(loc.x, loc.y, loc.z);

	glm::vec3 camLoc = camGo->transform->getPosition();
	Vector3 camLocation(camLoc.x, camLoc.y, camLoc.z);
	
	//Vector3 location(0, 0, 0);
	//CAudioEngine soundTest;
	//soundTest->Ini
	//LoadSound(const std::string& strSoundName, bool b3d, bool bLooping, bool bStream)
	//parameters about streaming, looping, and whether or not it's a 3D sound
	string soundPath1 = "../Assets/sounds/inception_sound.mp3";
	std::shared_ptr<AudioComponent> soundTest(new AudioComponent());
	cube->AddComponent(soundTest);

	soundTest->Load3D(soundPath1, true, true, true);

	soundTest->Play(soundPath1, camLocation, 3);

	//set the postion of the camera and one from the object for the sound to actually


	SceneManager::getInstance().SetActiveScene(scene);

	//SceneManager::getInstance().SaveSceneToFile(*scene);
}