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
		TEST_METHOD(Save_and_Load_GameObject)
		{
			// Loading Test Scene with test game object
			SceneManager::CreateManager();
			Scene_ptr savingTestScene = SceneManager::getInstance().CreateNewScene("TestScene");
			SceneManager::getInstance().SetActiveScene(savingTestScene);
			savingTestScene->CreateGameObject("TestObj");
			SceneManager::getInstance().SaveSceneToFile(*savingTestScene);

			// Loading Test Scene with test game object
			Scene_ptr loadedScene(new Scene("TestScene"));
			bool exists = SceneManager::getInstance().LoadSceneFromFileByName(*loadedScene, "TestScene");
			if (exists)
			{
				// if scene file exist

				// check if the same gameobj from the saved scene is loaded
				Assert::IsTrue(loadedScene->allGameObjects[0]->name == "TestObj");
			}
			else
			{
				// if scene file does not exist
				Assert::Fail();
			}
		}
	};
}