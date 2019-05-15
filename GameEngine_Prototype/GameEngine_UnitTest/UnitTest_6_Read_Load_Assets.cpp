#include "stdafx.h"
#include "CppUnitTest.h"
//
#include "XEngine.h"
#include "Transform.h"
#include "MeshRenderer.h"
using namespace XEngine;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace XEngine_UnitTest
{
	// each test method needs to close application to complete test
	TEST_CLASS(Read_Load_Assets)
	{
	public:
		TEST_METHOD(Read_Load_3D_Models)
		{
			// Arrange
			ENGINE_INITIALIZE();
			ENGINE_MAIN();

			// Act
			std::shared_ptr<XEngine::MeshRenderer> cube(new XEngine::MeshRenderer("3Dmodel/MetalCrate/cube.obj|Cube"));


			// Assert
			Assert::IsTrue(cube->mesh != nullptr);
			Assert::IsTrue(cube->material != nullptr);
		}

		TEST_METHOD(Read_Load_Images)
		{
			// Arrange
			ENGINE_INITIALIZE();
			ENGINE_MAIN();

			// Act
			Texture* tex = AssetManager::getInstance().textureLib.LoadAsset("../../DemoProject/Assets/textures/MetalCrate.png");

			// Assert
			Assert::IsTrue(tex->id == 0);
			Assert::IsTrue(strcmp(tex->path.c_str(), "../../DemoProject/Assets/textures/MetalCrate.png") == 0 );
		}

		TEST_METHOD(Read_Load_Audio)
		{
			// Arrange
			ENGINE_INITIALIZE();
			ENGINE_MAIN();

			// Act
			//GameObject_ptr obj = std::shared_ptr<GameObject>(new GameObject("Obj"));
			std::shared_ptr<AudioComponent> aud(new AudioComponent());
			aud->soundPath = "../Assets/sounds/foghorn.wav";

			// Assert
			Assert::IsTrue(aud->soundPath == "../Assets/sounds/foghorn.wav");
		}
	};
}