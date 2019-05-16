#include "stdafx.h"
#include "CppUnitTest.h"
//
#include "XEngine.h"
#include "Transform.h"
using namespace XEngine;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace XEngine_UnitTest
{
	TEST_CLASS(Game_Obj_Transform_Test)
	{
	public:
		
		// Test transform exists
		TEST_METHOD(Transform_Exist_In_GameObj) 
		{
			// Arrange
			GameObject_ptr go = std::shared_ptr<GameObject>(new GameObject());

			// Act
			Transform* t = go->transform;

			// Assert
			Assert::IsTrue(t != nullptr);
		}

		// Test default transform position
		TEST_METHOD(Transform_Default_Position)
		{
			// Arrange
			GameObject_ptr go = std::shared_ptr<GameObject>(new GameObject());
			
			// Act
			glm::vec3 position = go->transform->getPosition();
			glm::quat rotation = go->transform->getRotation();
			glm::vec3 scale = go->transform->getLocalScale();

			// Assert
			Assert::AreEqual(0.0f, position.x);// make sure the position is at (0, 0, 0)
			Assert::AreEqual(0.0f, position.y);
			Assert::AreEqual(0.0f, position.z);
			Assert::AreEqual(0.0f, rotation.x);// make sure the rotation is at (0, 0, 0)
			Assert::AreEqual(0.0f, rotation.y);
			Assert::AreEqual(0.0f, rotation.z);
			Assert::AreEqual(1.0f, scale.x);// make sure the scale is at (1, 1, 1)
			Assert::AreEqual(1.0f, scale.y);
			Assert::AreEqual(1.0f, scale.z);
		}

		// Test change position transform
		TEST_METHOD(Transform_Change_Position)
		{
			// Arrange
			GameObject_ptr go = std::shared_ptr<GameObject>(new GameObject());
			glm::vec3 oldPosition = go->transform->getPosition();
			
			// Act
			go->transform->setPosition(10, -3, 100);
			glm::vec3 newPosition = go->transform->getPosition();

			// Assert
			Assert::IsTrue(oldPosition != newPosition);// new position does should not match the old position
			Assert::AreEqual(10.0f, newPosition.x);// new positons are set correctly
			Assert::AreEqual(-3.0f, newPosition.y);
			Assert::AreEqual(100.0f, newPosition.z);
		}

		// Test change rotations transform
		TEST_METHOD(Transform_Change_Rotation)
		{
			// Arrange
			GameObject_ptr go = std::shared_ptr<GameObject>(new GameObject());
			glm::quat oldRotation = go->transform->getRotation();

			// Act
			glm::quat newRot(0, 10, -5, 100);
			go->transform->setRotation(newRot);
			glm::quat newRotation = go->transform->getRotation();

			// Assert
			Assert::IsTrue(oldRotation != newRotation);// new position does should not match the old position
			Assert::AreEqual(10.0f, newRotation.x);// new positons are set correctly
			Assert::AreEqual(-5.0f, newRotation.y);
			Assert::AreEqual(100.0f, newRotation.z);
		}

		// Test change scale transform
		TEST_METHOD(Transform_Change_Scale)
		{
			// Arrange
			GameObject_ptr go = std::shared_ptr<GameObject>(new GameObject());
			glm::vec3 oldScale = go->transform->getLocalScale();

			// Act
			go->transform->setLocalScale(100, 10, -4);
			glm::vec3 newScale = go->transform->getLocalScale();

			// Assert
			Assert::IsTrue(oldScale != newScale);// new scale does should not match the old scale
			Assert::AreEqual(100.0f, newScale.x);// new scale are set correctly
			Assert::AreEqual(10.0f, newScale.y);
			Assert::AreEqual(-4.0f, newScale.z);
		}
	};
}