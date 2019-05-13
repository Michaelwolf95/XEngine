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
	TEST_CLASS(GameObject_Component)
	{
	public:
		TEST_METHOD(Add_Component)
		{
			// Arrange
			GameObject_ptr obj = std::shared_ptr<GameObject>(new GameObject("Obj"));

			// Act
			std::shared_ptr<TestMoverComponent> test(new TestMoverComponent());
			obj->AddComponent(test);

			// Assert
			Assert::IsTrue(obj->components.size() == 1);
		}

		TEST_METHOD(Delete_Component)
		{
			// Arrange
			GameObject_ptr obj = std::shared_ptr<GameObject>(new GameObject("Obj"));

			// Act Part 1
			std::shared_ptr<TestMoverComponent> test(new TestMoverComponent());
			obj->AddComponent(test);

			// Assert Part 1
			Assert::IsTrue(obj->components.size() == 1);

			// Act Part 2
			obj->RemoveComponent(test);

			// Assert Part 2
			Assert::IsTrue(obj->components.size() == 0);
		}
	};
}