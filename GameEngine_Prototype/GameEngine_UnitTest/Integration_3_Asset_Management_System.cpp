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
	TEST_CLASS(Asset_Management_System)
	{
	public:
		TEST_METHOD(Test_Material_Library)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("Test_AssetManagement_System");

			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();

			// Act Part 1
			Material* savedMat = AssetManager::getInstance().materialLib.GetAsset("mat");

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

			// Act Part 2
			Material* loadedMat = AssetManager::getInstance().materialLib.GetAsset("mat");

			// Assert: if values are the same
			Assert::IsTrue(savedMat == loadedMat);
		}

		TEST_METHOD(Test_Mesh_Library)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("Test_AssetManagement_System");

			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();

			// Act Part 1
			Mesh* savedMesh = AssetManager::getInstance().meshLib.GetAsset("../Assets/3Dmodel/MetalCrate/cube.obj", "Cube");

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

			// Act Part 2
			Mesh* loadedMesh = AssetManager::getInstance().meshLib.GetAsset("../Assets/3Dmodel/MetalCrate/cube.obj", "Cube");

			// Assert: if values are the same
			Assert::IsTrue(savedMesh == loadedMesh);
		}

		TEST_METHOD(Test_Model_Library)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("Test_AssetManagement_System");

			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();

			// Act Part 1
			Model* savedModel = AssetManager::getInstance().modelLib.GetAsset("../Assets/3Dmodel/MetalCrate/cube.obj|Cube");

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

			// Act Part 2
			Model* loadedModel = AssetManager::getInstance().modelLib.GetAsset("../Assets/3Dmodel/MetalCrate/cube.obj|Cube");
			
			// Assert: if values are the same
			Assert::IsTrue(savedModel == loadedModel);
		}

		TEST_METHOD(Test_Shader_Library)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("Test_AssetManagement_System");

			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();

			// Act Part 1
			Shader* savedShader = AssetManager::getInstance().shaderLib.GetAsset("../Assets/Shaders/multilights.shader", "");

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

			// Act Part 2
			Shader* loadedShader = AssetManager::getInstance().shaderLib.GetAsset("../Assets/Shaders/multilights.shader", "");

			// Assert: if values are the same
			Assert::IsTrue(savedShader == loadedShader);
		}

		TEST_METHOD(Test_Texture_Library)
		{
			// Arrange
			ENGINE_INITIALIZE();
			Scene_ptr scene = SceneManager::getInstance().CreateNewScene("Test_AssetManagement_System");

			SceneManager::getInstance().SetActiveScene(scene);
			SceneManager::getInstance().SaveActiveScene();

			// Act Part 1
			Texture savedTexture = AssetManager::getInstance().textureLib.GetAsset("../Assets/textures/container.jpg");

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

			// Act Part 2
			Texture loadedTexure = AssetManager::getInstance().textureLib.GetAsset("../Assets/textures/container.jpg");

			// Assert: if values are the same
			Assert::IsTrue(savedTexture.id == loadedTexure.id);
			Assert::IsTrue(savedTexture.path == loadedTexure.path);
			Assert::IsTrue(savedTexture.type == loadedTexure.type);
		}
	};
}