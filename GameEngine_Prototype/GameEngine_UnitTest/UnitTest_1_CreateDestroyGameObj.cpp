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
		// Test Creating Game Object
		TEST_METHOD(Create_Game_Obj)
		{
			// Arrange
			Scene_ptr testScene(new Scene("Test Scene"));

			// Act
			GameObject_ptr testObj = testScene->CreateGameObject("Test Object");
			
			// Assert
			Assert::IsTrue(testObj != nullptr);// make sure game obj is not a null ptr
			Assert::AreEqual(testObj->name.c_str(), "Test Object");// make sure the game obj has the same name
			Assert::IsTrue(testScene->rootGameObjects[0] == testObj);// make sure it is the same instance of the game obj
		}

		//Test Destroy Object
		TEST_METHOD(Destroy_Game_Obj)
		{
			// Arrange
			Scene_ptr testScene(new Scene("Test Scene"));
			GameObject_ptr testObj = testScene->CreateGameObject("Test Object");
			Assert::IsFalse(testScene->rootGameObjects.empty());

			// Act
			testScene->DeleteGameObject(testObj);

			// Assert
			Assert::IsTrue(testScene->rootGameObjects.empty());// make sure test game obj is not in test scene
		}
	};
}