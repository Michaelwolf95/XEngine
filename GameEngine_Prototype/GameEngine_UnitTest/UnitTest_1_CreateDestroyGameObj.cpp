#include "stdafx.h"
#include "CppUnitTest.h"
//
#include "XEngine.h"
#include "Transform.h"
using namespace XEngine;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace XEngine_UnitTest
{
	TEST_CLASS(Game_Obj_Test)
	{
	public:
		TEST_METHOD(Create_Game_Obj)
		{
			Scene_ptr testScene(new Scene("Test Scene"));

			GameObject_ptr testObj = testScene->CreateGameObject("Test Object");
			
			// make sure game obj is not a null ptr
			Assert::IsTrue(testObj != nullptr);

			// make sure the game obj has the same name
			Assert::AreEqual(testObj->name.c_str(), "Test Object");

			// make sure it is the same instance of the game obj
			Assert::IsTrue(testScene->rootGameObjects[0] == testObj);
		}

		TEST_METHOD(Destroy_Game_Obj)
		{
			Scene_ptr testScene(new Scene("Test Scene"));

			GameObject_ptr testObj = testScene->CreateGameObject("Test Object");

			// make sure game obj is in scene
			Assert::IsFalse(testScene->rootGameObjects.empty());

			testScene->DeleteGameObject(testObj);

			// make sure test game obj is not in test scene
			Assert::IsTrue(testScene->rootGameObjects.empty());
		}
	};
}