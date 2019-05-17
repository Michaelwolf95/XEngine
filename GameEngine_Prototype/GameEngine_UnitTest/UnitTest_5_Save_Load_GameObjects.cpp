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
	TEST_CLASS(Save_Load_GameObject)
	{
	public:
		// Test save and load gameobject
		TEST_METHOD(Save_and_Load_GameObject)
		{
			// Arrange
			SceneManager::CreateManager();
			Scene_ptr savingTestScene = SceneManager::getInstance().CreateNewScene("TestScene");
			SceneManager::getInstance().SetActiveScene(savingTestScene);
			savingTestScene->CreateGameObject("TestObj");

			// Act: Save
			SceneManager::getInstance().SaveSceneToFile(*savingTestScene);

			// Act: Load
			Scene_ptr loadedScene(new Scene("TestScene"));
			bool exists = SceneManager::getInstance().LoadSceneFromFileByName(*loadedScene, "TestScene");
			
			if (exists)
			{
				// Assert
				Assert::IsTrue(loadedScene->allGameObjects[0]->name == "TestObj");
			}
			else
			{
				// Assert fail
				Assert::Fail();
			}
		}
	};
}