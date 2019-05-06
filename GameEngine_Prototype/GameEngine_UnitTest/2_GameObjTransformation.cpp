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
		TEST_METHOD(Transform_Exist_In_GameObj) 
		{
			GameObject_ptr go = std::shared_ptr<GameObject>(new GameObject());
			Transform* t = go->transform;

			Assert::IsTrue(t != nullptr);
		}

		TEST_METHOD(Transform_Default_Position)
		{
			GameObject_ptr go = std::shared_ptr<GameObject>(new GameObject());
			
			// make sure the position is at (0, 0, 0)
			glm::vec3 position = go->transform->getPosition();
			Assert::AreEqual(0.0f, position.x);
			Assert::AreEqual(0.0f, position.y);
			Assert::AreEqual(0.0f, position.z);

			// make sure the rotation is at (0, 0, 0)
			glm::quat rotation = go->transform->getRotation();
			Assert::AreEqual(0.0f, rotation.x);
			Assert::AreEqual(0.0f, rotation.y);
			Assert::AreEqual(0.0f, rotation.z);

			// make sure the scale is at (1, 1, 1)
			glm::vec3 scale = go->transform->getLocalScale();
			Assert::AreEqual(1.0f, scale.x);
			Assert::AreEqual(1.0f, scale.y);
			Assert::AreEqual(1.0f, scale.z);
		}

		TEST_METHOD(Transform_Change_Position)
		{
			GameObject_ptr go = std::shared_ptr<GameObject>(new GameObject());
			
			glm::vec3 oldPosition = go->transform->getPosition();

			go->transform->setPosition(10, -3, 100);

			glm::vec3 newPosition = go->transform->getPosition();

			// new position does should not match the old position
			Assert::IsTrue(oldPosition != newPosition);

			// new positons are set correctly
			Assert::AreEqual(10.0f, newPosition.x);
			Assert::AreEqual(-3.0f, newPosition.y);
			Assert::AreEqual(100.0f, newPosition.z);
		}

		TEST_METHOD(Transform_Change_Rotation)
		{
			GameObject_ptr go = std::shared_ptr<GameObject>(new GameObject());

			glm::quat oldRotation = go->transform->getRotation();

			// setting new rotation (ignoring w variable change)
			glm::quat newRot(0, 10, -5, 100);
			go->transform->setRotation(newRot);

			glm::quat newRotation = go->transform->getRotation();

			// new position does should not match the old position
			Assert::IsTrue(oldRotation != newRotation);

			// new positons are set correctly
			Assert::AreEqual(10.0f, newRotation.x);
			Assert::AreEqual(-5.0f, newRotation.y);
			Assert::AreEqual(100.0f, newRotation.z);
		}

		TEST_METHOD(Transform_Change_Scale)
		{
			GameObject_ptr go = std::shared_ptr<GameObject>(new GameObject());

			glm::vec3 oldScale = go->transform->getLocalScale();

			// setting new scale
			go->transform->setLocalScale(100, 10, -4);

			glm::vec3 newScale = go->transform->getLocalScale();

			// new scale does should not match the old scale
			Assert::IsTrue(oldScale != newScale);

			// new scale are set correctly
			Assert::AreEqual(100.0f, newScale.x);
			Assert::AreEqual(10.0f, newScale.y);
			Assert::AreEqual(-4.0f, newScale.z);
		}
	};
}