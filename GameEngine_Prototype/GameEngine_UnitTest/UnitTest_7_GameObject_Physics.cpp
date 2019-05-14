#include "stdafx.h"
#include "CppUnitTest.h"
//
#include "XEngine.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Rigidbody.h"
#include "MeshRenderer.h"

using namespace XEngine;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace XEngine_UnitTest
{
	TEST_CLASS(GameObject_Physics)
	{
	public:
		TEST_METHOD(Collision_Test)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestingPhysics");
			
			// camera
			GameObject_ptr cam = scene->CreateGameObject("Camere");
			std::shared_ptr<CameraComponent> cameraComponent(new CameraComponent());
			cam->AddComponent(cameraComponent);
			cam->transform->setPosition(-1, 7, -40);

			GameObject_ptr floor = scene->CreateGameObject("Floor");
			std::shared_ptr<XEngine::MeshRenderer> crate1(new XEngine::MeshRenderer("3Dmodel/MetalCrate/cube.obj|Cube"));
			floor->AddComponent(crate1);
			floor->transform->setLocalScale(10, 1, 10); // make floor wide
			shared_ptr<Rigidbody> floorRigid(new Rigidbody());
			floorRigid->isKinematic = true;//make floor rigid
			floor->AddComponent(floorRigid);
			shared_ptr<BoxCollider> floorCollide(new BoxCollider());
			floor->AddComponent(floorCollide);

			GameObject_ptr box = scene->CreateGameObject("Ball");
			std::shared_ptr<XEngine::MeshRenderer> crate2(new XEngine::MeshRenderer("3Dmodel/MetalCrate/cube.obj|Cube"));
			box->AddComponent(crate2);
			box->transform->setPosition(0,10,0); //move ball up so it can land
			shared_ptr<Rigidbody> boxRigid(new Rigidbody());
			box->AddComponent(boxRigid);
			shared_ptr<BoxCollider> boxCollide(new BoxCollider());
			box->AddComponent(boxCollide);
			
			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();
			
			int collisions = 0;

			// FRAME LOOP
			while (!ApplicationManager::getInstance().CheckIfAppShouldClose())
			{
				ApplicationManager::getInstance().ApplicationStartUpdate();
				GameTime::getInstance().UpdateTime();
				Input::getInstance().UpdateInput();

				// MAIN UPDATE
				if (OnEngineUpdate != nullptr) OnEngineUpdate();
				SceneManager::getInstance().UpdateActiveScene();
				AudioManager::getInstance().UpdateAudio();

				PhysicsManager::getInstance().PhysicsUpdate();
				collisions = PhysicsManager::getInstance().dynamicsWorld->getDispatcher()->getNumManifolds();


				if (OnEnginePreRender != nullptr) OnEnginePreRender();

				RenderManager::getInstance().Render();

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			Assert::IsTrue(collisions > 0);
		}
	};
}