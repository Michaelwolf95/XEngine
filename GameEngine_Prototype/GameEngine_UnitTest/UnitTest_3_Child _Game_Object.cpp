#include "stdafx.h"
#include "CppUnitTest.h"
//
#include "XEngine.h"
#include "Transform.h"
using namespace XEngine;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace XEngine_UnitTest
{
	TEST_CLASS(Child_Game_Obj)
	{
	public:
		// Test child attachment
		TEST_METHOD(Child_Attachment)
		{
			// Arrange
			GameObject_ptr parent = std::shared_ptr<GameObject>(new GameObject("Parent"));
			GameObject_ptr child = std::shared_ptr<GameObject>(new GameObject("Child"));

			// Act
			child->transform->SetParent(parent->transform);

			// Assert
			Assert::IsTrue(parent->transform->GetChildCount() == 1);// make sure parent has only one parent
			Assert::IsTrue(child->transform->GetParent() == parent->transform);// make sure child has parent
			Assert::IsTrue(parent->GetChild(0)->transform == child->transform);// make sure parent has child
		}

		// Test child position change
		TEST_METHOD(Child_Position_Change)
		{
			// Arrange
			GameObject_ptr parent = std::shared_ptr<GameObject>(new GameObject("Parent"));
			GameObject_ptr child = std::shared_ptr<GameObject>(new GameObject("Child"));
			child->transform->setPosition(5, 5, 5);
			child->transform->SetParent(parent->transform);

			// Act
			parent->transform->setPosition(1, 10, 100);

			// Assert
			glm::vec3 childPosition = child->transform->getPosition();// get changed child position
			Assert::IsTrue(6 == childPosition.x);// make sure the child's position is changed accordingly
			Assert::IsTrue(15 == childPosition.y);
			Assert::IsTrue(105 == childPosition.z);
		}

		// Test Child rotation change
		TEST_METHOD(Child_Rotation_Change)
		{
			// Arrange
			GameObject_ptr parent = std::shared_ptr<GameObject>(new GameObject("Parent"));
			GameObject_ptr child = std::shared_ptr<GameObject>(new GameObject("Child"));
			glm::quat rotationChildChange(0, 5, 5, 5);
			child->transform->setRotation(rotationChildChange);
			child->transform->SetParent(parent->transform);

			// Act
			glm::quat rotationParentChange(0, 1, 10, 100);
			parent->transform->setRotation(rotationParentChange);
			glm::quat actualChildRotation = child->transform->getRotation();
			glm::quat expectedChildRotation = child->transform->getLocalRotation() * parent->transform->getRotation();

			// Assert
			Assert::IsTrue(expectedChildRotation.x == actualChildRotation.x);
			Assert::IsTrue(expectedChildRotation.y == actualChildRotation.y);
			Assert::IsTrue(expectedChildRotation.z == actualChildRotation.z);
		}

		// Test Child Scale change
		TEST_METHOD(Child_Scale_Change)
		{
			// Arrange
			GameObject_ptr parent = std::shared_ptr<GameObject>(new GameObject("Parent"));
			GameObject_ptr child = std::shared_ptr<GameObject>(new GameObject("Child"));
			child->transform->setLocalScale(5, 5, 5);
			child->transform->SetParent(parent->transform);

			// Act
			parent->transform->setLocalScale(1, 10, 100);
			glm::vec3 childScale = child->transform->getScale();

			// Assert
			Assert::IsTrue(5*1 == childScale.x);
			Assert::IsTrue(5*10 == childScale.y);
			Assert::IsTrue(5*100 == childScale.z);
		}
	};
}