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
	TEST_CLASS(Read_Load_Assets)
	{
	public:
		// need to close application to complete test
		TEST_METHOD(Read_Load_3D_Models)
		{
			// Arrange
			ENGINE_INITIALIZE();
			ENGINE_MAIN();

			// Act
			std::shared_ptr<XEngine::MeshRenderer> cube(new XEngine::MeshRenderer("3Dmodel/MetalCrate/cube.obj|Cube"));


			// Assert
			Assert::IsTrue(cube->mesh != nullptr);
			Assert::IsTrue(cube->material != nullptr);
		}
	};
}