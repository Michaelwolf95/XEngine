#include "stdafx.h"
#include "CppUnitTest.h"
//
#include "XEngine.h"
using namespace XEngine;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace XEngine_UnitTest
{

	TEST_CLASS(Transform_Test)
	{
	public:
		TEST_METHOD(Test_Test)
		{
			GameObject_ptr go = std::shared_ptr<GameObject>(new GameObject());
			Transform* t = go->transform;

			Assert::IsTrue(t != nullptr);
		}
	};
}