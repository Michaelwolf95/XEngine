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

void CreateTestScene_Donato()
{
	Scene* scene = new Scene("Test Scene for Donato");

	SceneManager::getInstance().SetActiveScene(scene);
}

void RunTestScene_Donato()
{
	CreateTestScene_Donato();
}