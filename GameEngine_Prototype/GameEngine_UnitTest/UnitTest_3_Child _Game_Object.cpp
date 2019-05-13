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
		TEST_METHOD(Child_Attachment)
		{
			GameObject_ptr parent = std::shared_ptr<GameObject>(new GameObject("Parent"));

			GameObject_ptr child = std::shared_ptr<GameObject>(new GameObject("Child"));

			child->transform->SetParent(parent->transform);

			// make sure parent has only one parent
			Assert::IsTrue(parent->transform->GetChildCount() == 1);

			// make sure child has parent
			Assert::IsTrue(child->transform->GetParent() == parent->transform);

			// make sure parent has child
			Assert::IsTrue(parent->GetChild(0)->transform == child->transform);
		}

		TEST_METHOD(Child_Position_Change)
		{
			GameObject_ptr parent = std::shared_ptr<GameObject>(new GameObject("Parent"));

			GameObject_ptr child = std::shared_ptr<GameObject>(new GameObject("Child"));

			// change child position
			child->transform->setPosition(5, 5, 5);

			// make parent-child relationship
			child->transform->SetParent(parent->transform);

			// change parent position
			parent->transform->setPosition(1, 10, 100);

			// get changed child position
			glm::vec3 childPosition = child->transform->getPosition();

			// make sure the child's position is changed accordingly
			Assert::IsTrue(6 == childPosition.x);
			Assert::IsTrue(15 == childPosition.y);
			Assert::IsTrue(105 == childPosition.z);
		}

		TEST_METHOD(Child_Rotation_Change)
		{
			GameObject_ptr parent = std::shared_ptr<GameObject>(new GameObject("Parent"));

			GameObject_ptr child = std::shared_ptr<GameObject>(new GameObject("Child"));

			// change child rotation
			glm::quat rotationChildChange(0, 5, 5, 5);
			child->transform->setRotation(rotationChildChange);

			// make parent-child relationship
			child->transform->SetParent(parent->transform);

			// change parent rotation
			glm::quat rotationParentChange(0, 1, 10, 100);
			parent->transform->setRotation(rotationParentChange);

			// get changed child rotation
			glm::quat actualChildRotation = child->transform->getRotation();

			// expected child rotation
			glm::quat expectedChildRotation = child->transform->getLocalRotation() * parent->transform->getRotation();

			// make sure the child's rotation is changed accordingly
			Assert::IsTrue(expectedChildRotation.x == actualChildRotation.x);
			Assert::IsTrue(expectedChildRotation.y == actualChildRotation.y);
			Assert::IsTrue(expectedChildRotation.z == actualChildRotation.z);
		}

		TEST_METHOD(Child_Scale_Change)
		{
			GameObject_ptr parent = std::shared_ptr<GameObject>(new GameObject("Parent"));

			GameObject_ptr child = std::shared_ptr<GameObject>(new GameObject("Child"));

			// change child scale
			child->transform->setLocalScale(5, 5, 5);

			// make parent-child relationship
			child->transform->SetParent(parent->transform);

			// change parent scale
			parent->transform->setLocalScale(1, 10, 100);

			// get changed child position
			glm::vec3 childScale = child->transform->getScale();

			// make sure the child's position is changed accordingly
			Assert::IsTrue(5*1 == childScale.x);
			Assert::IsTrue(5*10 == childScale.y);
			Assert::IsTrue(5*100 == childScale.z);
		}
	};
}