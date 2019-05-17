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

	TEST_CLASS(Input_Management_System)
	{
	public:

		// To Test, don't move mouse
		TEST_METHOD(Test_GetMousePosX)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestingPhysics");

			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();
			double posResult = 0;

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

				// Act
				posResult = Input::getInstance().GetMousePosX();

				RenderManager::getInstance().Render();

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert
			Assert::AreEqual(posResult, Input::getInstance().GetMousePosX());
		}

		// To Test, don't move mouse
		TEST_METHOD(Test_GetMousePosY)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestingPhysics");

			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();
			double posResult = 0;

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

				// Act
				posResult = Input::getInstance().GetMousePosY();

				RenderManager::getInstance().Render();

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert
			Assert::AreEqual(posResult, Input::getInstance().GetMousePosY());
		}

		TEST_METHOD(Test_GetMousePos) // Currently Failing
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestingPhysics");

			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();
			float xpos = 0;
			float ypos = 0;
			
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

				// Act
				xpos = Input::getInstance().GetMousePosX();
				ypos = Input::getInstance().GetMousePosY();

				RenderManager::getInstance().Render();

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert
			glm::vec2 posResult = glm::vec2(xpos, ypos);
			Assert::IsTrue(posResult == Input::getInstance().GetMousePos());
		}

		// To Test, move mouse horizontally
		TEST_METHOD(Test_GetDeltaPosX)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestingPhysics");

			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();
			double xpos = 0;
			double ypos = 0;

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

				// Act
				xpos = Input::getInstance().GetMousePosX();
				ypos = Input::getInstance().GetMousePosY();

				RenderManager::getInstance().Render();

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert
			Assert::IsTrue(0 < Input::getInstance().GetDeltaPosX());
		}

		// To Test, move mouse vertically
		TEST_METHOD(Test_GetDeltaPosY)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestingPhysics");

			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();
			double xpos = 0;
			double ypos = 0;

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

				// Act
				xpos = Input::getInstance().GetMousePosX();
				ypos = Input::getInstance().GetMousePosY();

				RenderManager::getInstance().Render();

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert
			Assert::IsTrue(0 < Input::getInstance().GetDeltaPosY());
		}

		// To test, move mouse around
		TEST_METHOD(Test_GetMouseDelta)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestingPhysics");
			glm::vec2 deltaMouse;
				
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

				// Act
				deltaMouse = Input::getInstance().GetMouseDelta();

				RenderManager::getInstance().Render();

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();
				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert
			Assert::IsTrue(0 < deltaMouse.x);
			Assert::IsTrue(0 < deltaMouse.y);
		}

		TEST_METHOD(Test_ScrollCallback)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestingPhysics");
			float xScrollOffset = 17.8;
			float yScrollOffset = 18.4;

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

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();

				// Act
				Input::getInstance()._scroll_callback(xScrollOffset, yScrollOffset);

				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert
			Assert::IsTrue(xScrollOffset == Input::getInstance().GetScrollOffsetX());
			Assert::IsTrue(yScrollOffset == Input::getInstance().GetScrollOffsetY());
		}

		TEST_METHOD(Test_GetScrollOffsetX)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestingPhysics");

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

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();

				// Act
				Input::getInstance()._scroll_callback(4.0, 4.5);

				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert
			Assert::IsTrue(4.0 == Input::getInstance().GetScrollOffsetX());
		}

		TEST_METHOD(Test_GetScrollOffsetY)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestingPhysics");

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

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();

				// Act
				Input::getInstance()._scroll_callback(4.0, 4.5);

				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Assert
			Assert::IsTrue(4.5 == Input::getInstance().GetScrollOffsetY());
		}

		TEST_METHOD(Test_GetMouseButtonDown)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestingPhysics");

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

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();

				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Act and Assert
			Assert::IsTrue(false == Input::getInstance().GetMouseButtonDown(1));
		}

		TEST_METHOD(Test_GetMouseButton)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestingPhysics");

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

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();

				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Act and Assert
			Assert::IsTrue(false == Input::getInstance().GetMouseButton(1));
		}

		TEST_METHOD(Test_GetMouseButtonUp)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestingPhysics");

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

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();

				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Act and Assert
			Assert::IsTrue(false == Input::getInstance().GetMouseButtonUp(1));
		}

		TEST_METHOD(Test_GetKeyDown)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestingPhysics");

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

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();

				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Act and Assert
			Assert::IsTrue(false == Input::getInstance().GetKeyDown(1));
		}

		TEST_METHOD(Test_GetKey)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestingPhysics");

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

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();

				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Act and Assert
			Assert::IsTrue(false == Input::getInstance().GetKey(1));
		}

		TEST_METHOD(Test_GetKeyUp)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("TestingPhysics");

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

				if (OnEnginePostRender != nullptr) OnEnginePostRender();

				Input::getInstance().EndUpdateFrame();

				ApplicationManager::getInstance().ApplicationEndUpdate();
			}

			if (OnApplicationClose != nullptr) OnApplicationClose();

			ApplicationManager::getInstance().CloseApplication();

			// Act and Assert
			Assert::IsTrue(false == Input::getInstance().GetKeyUp(1));
		}
	};
}