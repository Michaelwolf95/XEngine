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
	TEST_CLASS(Scene_Management_System)
	{
	public:
		TEST_METHOD(Test_GetActiveScene)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr sceneExpected = SceneManager::getInstance().CreateNewScene("Test_SceneManagement_System");
			Scene_ptr sceneResult = nullptr;

			SceneManager::getInstance().SetActiveScene(sceneExpected);
			SceneManager::getInstance().SaveActiveScene();

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

				// Act 
				sceneResult = SceneManager::getInstance().GetActiveScene();

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert: if values are the same
			Assert::IsTrue(sceneExpected == sceneResult);
		}

		TEST_METHOD(Test_SetActiveScene)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("Bubble");
			Scene_ptr sceneExpected(new Scene("Test_SceneManagement_System"));
			Scene_ptr sceneResult = nullptr;

			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();

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

				// Act 
				SceneManager::getInstance().SetActiveScene(sceneExpected);
				sceneResult = SceneManager::getInstance().GetActiveScene();

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert: if values are the same
			Assert::AreEqual(sceneResult->name.c_str(), "Test_SceneManagement_System");
		}

		TEST_METHOD(Test_CreateNewScene)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("Bubble");
			Scene_ptr sceneResult = nullptr;

			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();

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

				// Act 
				sceneResult = SceneManager::getInstance().CreateNewScene("Test_SceneManagement_System");

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert: if values are the same
			Assert::AreEqual(sceneResult->name.c_str(), "Test_SceneManagement_System");
		}

		TEST_METHOD(Test_SaveActiveScene_LoadActiveScene)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("Bubble");
			Scene_ptr sceneExpected(new Scene("Test_SceneManagement_System"));
			Scene_ptr sceneResult(new Scene("Timmy"));

			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();

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

				// Act 
				SceneManager::getInstance().SaveSceneToFile(*sceneExpected);
				SceneManager::getInstance().LoadSceneFromFileByName(*sceneResult, "Test_SceneManagement_System");

				
				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert: if values are the same
			Assert::AreEqual(sceneResult->name.c_str(), "Test_SceneManagement_System");
		}

		TEST_METHOD(Test_LoadandActivateScene)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("Bubble");
			Scene_ptr scene2(new Scene("Test_SceneManagement_System"));
			Scene_ptr sceneResult = nullptr;
			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();

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

				// Act 
				SceneManager::getInstance().SaveSceneToFile(*scene2);
				bool exist = SceneManager::getInstance().LoadAndActivateSceneFromFile("../Assets/Scenes/Test_SceneManagement_System.scene");
				sceneResult = SceneManager::getInstance().GetActiveScene();

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert: if values are the same
			Assert::AreEqual(sceneResult->name.c_str(), "Test_SceneManagement_System");
		}

		TEST_METHOD(Test_UnloadActiveScene)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("Test_SceneManagement_System");
			Scene_ptr sceneResult = nullptr;
			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();

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

				// Act
				SceneManager::getInstance().UnloadActiveScene();
				sceneResult = SceneManager::getInstance().GetActiveScene();

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert: if values are the same
			Assert::IsTrue(sceneResult == nullptr);
		}

		TEST_METHOD(Test_ReloadActiveScene)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("Test_SceneManagement_System");
			Scene_ptr sceneResult = nullptr;
			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();

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

				// Act
				SceneManager::getInstance().ReloadSceneFromFile();
				sceneResult = SceneManager::getInstance().GetActiveScene();

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert: if values are the same
			Assert::AreEqual(sceneResult->name.c_str(), "Test_SceneManagement_System");
		}

	};
}