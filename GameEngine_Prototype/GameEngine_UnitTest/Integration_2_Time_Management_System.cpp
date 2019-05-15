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
	TEST_CLASS(Time_Management_System)
	{
	public:
		// Test update time
		TEST_METHOD(Test_UpdateTime)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestGameObject_Component_System");

			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();

			float currentPre = 0.0f;
			float deltaPre = 0.0f;
			float lastPre = 0.0f;
			float currentPost = 0.0f;
			float deltaPost = 0.0f;
			float lastPost = 0.0f;

			while (!ApplicationManager::getInstance().CheckIfAppShouldClose())
			{
				ApplicationManager::getInstance().ApplicationStartUpdate();

				// Act
				currentPre = GameTime::getInstance().currentTime;
				deltaPre = GameTime::getInstance().deltaTime;
				lastPre = GameTime::getInstance().timeLastFrame;
				GameTime::getInstance().UpdateTime();
				currentPost = GameTime::getInstance().currentTime;
				deltaPost = GameTime::getInstance().deltaTime;
				lastPost = GameTime::getInstance().timeLastFrame;

				Input::getInstance().UpdateInput();

				if (OnEngineUpdate != nullptr) OnEngineUpdate();
				SceneManager::getInstance().UpdateActiveScene();
				AudioManager::getInstance().UpdateAudio();

				PhysicsManager::getInstance().PhysicsUpdate();

				if (OnEnginePreRender != nullptr) OnEnginePreRender();

				RenderManager::getInstance().Render();

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert: check update values
			Assert::AreEqual(currentPost-currentPre, deltaPost); // checking delta time 
			Assert::AreEqual(currentPost, lastPost); // checking last time frame
		}

		// Test ToggleFPS and IsCounting
		TEST_METHOD(Test_ToggleFPS_IsCounting)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestGameObject_Component_System");

			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();

			while (!ApplicationManager::getInstance().CheckIfAppShouldClose())
			{
				ApplicationManager::getInstance().ApplicationStartUpdate();

				// Act
				if (!GameTime::getInstance().IsCounting())
					GameTime::getInstance().ToggleFPS();

				Input::getInstance().UpdateInput();

				if (OnEngineUpdate != nullptr) OnEngineUpdate();
				SceneManager::getInstance().UpdateActiveScene();
				AudioManager::getInstance().UpdateAudio();

				PhysicsManager::getInstance().PhysicsUpdate();

				if (OnEnginePreRender != nullptr) OnEnginePreRender();

				RenderManager::getInstance().Render();

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert: check update values
			Assert::IsTrue(GameTime::getInstance().IsCounting());
		}
	};
}