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
	TEST_CLASS(GameObject_Component_System)
	{
	public:
		// Test Adding Game Object in scene
		TEST_METHOD(Scene_Add_GameObject)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestGameObject_Component_System");

			// Act 
			scene->CreateGameObject("test");
			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();

			int results = 0;
			while (!ApplicationManager::getInstance().CheckIfAppShouldClose())
			{
				ApplicationManager::getInstance().ApplicationStartUpdate();
				GameTime::getInstance().UpdateTime();
				Input::getInstance().UpdateInput();

				if (OnEngineUpdate != nullptr) OnEngineUpdate();
				SceneManager::getInstance().UpdateActiveScene();
				AudioManager::getInstance().UpdateAudio();

				PhysicsManager::getInstance().PhysicsUpdate();

				if (OnEnginePreRender != nullptr) OnEnginePreRender();

				RenderManager::getInstance().Render();

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				// Act
				results = SceneManager::getInstance().GetActiveScene()->rootGameObjects.size();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert: Scene has at least one gameobject
			Assert::IsTrue(results == 1);
		}

		// Test removing gameobject in scene
		TEST_METHOD(Scene_Remove_GameObject)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestGameObject_Component_System");

			// Act 
			GameObject_ptr obj1 = scene->CreateGameObject("test");
			GameObject_ptr obj2 = scene->CreateGameObject("test2");
			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();

			int results = 0;
			while (!ApplicationManager::getInstance().CheckIfAppShouldClose())
			{
				ApplicationManager::getInstance().ApplicationStartUpdate();
				GameTime::getInstance().UpdateTime();
				Input::getInstance().UpdateInput();

				if (OnEngineUpdate != nullptr) OnEngineUpdate();
				SceneManager::getInstance().UpdateActiveScene();
				AudioManager::getInstance().UpdateAudio();

				PhysicsManager::getInstance().PhysicsUpdate();

				if (OnEnginePreRender != nullptr) OnEnginePreRender();

				RenderManager::getInstance().Render();

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				// Act
				SceneManager::getInstance().GetActiveScene()->DeleteGameObject(obj2);
				results = SceneManager::getInstance().GetActiveScene()->rootGameObjects.size();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert: Scene has at 1 gameobject, not 2
			Assert::IsTrue(results == 1);
		}

		// Test adding component in gameobject
		TEST_METHOD(GameObject_Add_Component)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestGameObject_Component_System");

			// Act 
			GameObject_ptr obj1 = scene->CreateGameObject("test");
			std::shared_ptr<TestMoverComponent> move(new TestMoverComponent());
			obj1->AddComponent(move);
			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();

			int results = 0;
			while (!ApplicationManager::getInstance().CheckIfAppShouldClose())
			{
				ApplicationManager::getInstance().ApplicationStartUpdate();
				GameTime::getInstance().UpdateTime();
				Input::getInstance().UpdateInput();

				if (OnEngineUpdate != nullptr) OnEngineUpdate();
				SceneManager::getInstance().UpdateActiveScene();
				AudioManager::getInstance().UpdateAudio();

				PhysicsManager::getInstance().PhysicsUpdate();

				if (OnEnginePreRender != nullptr) OnEnginePreRender();

				RenderManager::getInstance().Render();

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				// Act
				results = SceneManager::getInstance().GetActiveScene()->rootGameObjects[0]->components.size();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert: GameObject Component Size is 1
			Assert::IsTrue(results == 1);
		}

		// Test remove component from game object
		TEST_METHOD(GameObject_Remove_Component)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestGameObject_Component_System");

			// Act 
			GameObject_ptr obj1 = scene->CreateGameObject("test");
			std::shared_ptr<TestMoverComponent> move(new TestMoverComponent());
			std::shared_ptr<TestMoverComponent> move2(new TestMoverComponent());
			obj1->AddComponent(move);
			obj1->AddComponent(move2);
			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();

			int results = 0;
			while (!ApplicationManager::getInstance().CheckIfAppShouldClose())
			{
				ApplicationManager::getInstance().ApplicationStartUpdate();
				GameTime::getInstance().UpdateTime();
				Input::getInstance().UpdateInput();

				if (OnEngineUpdate != nullptr) OnEngineUpdate();
				SceneManager::getInstance().UpdateActiveScene();
				AudioManager::getInstance().UpdateAudio();

				PhysicsManager::getInstance().PhysicsUpdate();

				if (OnEnginePreRender != nullptr) OnEnginePreRender();

				RenderManager::getInstance().Render();

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				// Act
				SceneManager::getInstance().GetActiveScene()->rootGameObjects[0]->RemoveComponent(move);
				results = SceneManager::getInstance().GetActiveScene()->rootGameObjects[0]->components.size();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert: GameObject Component Size is 1, not 2
			Assert::IsTrue(results == 1);
		}

		// Test adding child gameobject
		TEST_METHOD(GameObject_Add_Child_GameObject)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestGameObject_Component_System");

			// Act 
			GameObject_ptr parent = scene->CreateGameObject("parent");
			GameObject_ptr child = scene->CreateGameObject("child");
			child->transform->SetParent(parent->transform);

			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();

			int results = 0;
			while (!ApplicationManager::getInstance().CheckIfAppShouldClose())
			{
				ApplicationManager::getInstance().ApplicationStartUpdate();
				GameTime::getInstance().UpdateTime();
				Input::getInstance().UpdateInput();

				if (OnEngineUpdate != nullptr) OnEngineUpdate();
				SceneManager::getInstance().UpdateActiveScene();
				AudioManager::getInstance().UpdateAudio();

				PhysicsManager::getInstance().PhysicsUpdate();

				if (OnEnginePreRender != nullptr) OnEnginePreRender();

				RenderManager::getInstance().Render();

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				// Act
				results = SceneManager::getInstance().GetActiveScene()->rootGameObjects[0]->transform->GetChildCount();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert: GameObject Component Size is 1
			Assert::IsTrue(results == 1);
		}

		// Test removing child game object
		TEST_METHOD(GameObject_Remove_Child_GameObject)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestGameObject_Component_System");

			// Act 
			GameObject_ptr parent = scene->CreateGameObject("parent");
			GameObject_ptr child = scene->CreateGameObject("child");
			GameObject_ptr child2 = scene->CreateGameObject("child2");
			child->transform->SetParent(parent->transform);
			child2->transform->SetParent(parent->transform);

			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();

			int results = 0;
			while (!ApplicationManager::getInstance().CheckIfAppShouldClose())
			{
				ApplicationManager::getInstance().ApplicationStartUpdate();
				GameTime::getInstance().UpdateTime();
				Input::getInstance().UpdateInput();

				if (OnEngineUpdate != nullptr) OnEngineUpdate();
				SceneManager::getInstance().UpdateActiveScene();
				AudioManager::getInstance().UpdateAudio();

				PhysicsManager::getInstance().PhysicsUpdate();

				if (OnEnginePreRender != nullptr) OnEnginePreRender();

				RenderManager::getInstance().Render();

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				// Act
				child2->transform->SetParent(nullptr);
				results = SceneManager::getInstance().GetActiveScene()->rootGameObjects[0]->transform->GetChildCount();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert: GameObject Component Size is 1, not 2
			Assert::IsTrue(results == 1);
		}
	};
}