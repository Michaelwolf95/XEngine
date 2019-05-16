#include "stdafx.h"
#include "CppUnitTest.h"
//
#include "XEngine.h"
using namespace XEngine;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace XEngine_UnitTest
{	

	TEST_CLASS(User_Input)
	{
	public:
		// Test getting Mouse position x value
		TEST_METHOD(GetMousePosX)
		{
			// Arrange
			Input::CreateManager();

			// Act and Assert
			Assert::AreEqual(0.0, Input::getInstance().GetMousePosX());
		}

		// Test getting Mouse position y value
		TEST_METHOD(GetMousePosY)
		{
			// Arrange
			Input::CreateManager();

			// Act and Assert
			Assert::AreEqual(0.0, Input::getInstance().GetMousePosY());
		}

		// Test getting Mouse position value
		TEST_METHOD(GetMousePos)
		{
			// Arrange
			Input::CreateManager();

			double xpos = 13.5;
			double ypos = 18.5;
			glm::vec2 mousePos = glm::vec2(xpos, ypos);
			Input::getInstance()._mouse_callback(xpos, ypos);

			// Act and Assert
			Assert::IsTrue(mousePos == Input::getInstance().GetMousePos());
		}

		// Test getting Mouse delta position x value
		TEST_METHOD(GetDeltaPosX)
		{
			// Arrange
			Input::CreateManager();

			double xpos = 12.5;
			double ypos = 11.0;
			Input::getInstance()._mouse_callback(xpos, ypos);

			double xpos2 = 18.5;
			double ypos2 = 11.0;
			Input::getInstance()._mouse_callback(xpos2, ypos2);

			// Act and Assert
			Assert::AreEqual(-6.0, Input::getInstance().GetDeltaPosX());
		}

		// Test getting Mouse delta position y value
		TEST_METHOD(GetDeltaPosY)
		{
			// Arrange
			Input::CreateManager();

			double xpos = 11.0;
			double ypos = 12.5;
			Input::getInstance()._mouse_callback(xpos, ypos);

			double xpos2 = 11.0;
			double ypos2 = 18.5;
			Input::getInstance()._mouse_callback(xpos2, ypos2);

			// Act and Assert
			Assert::AreEqual(6.0, Input::getInstance().GetDeltaPosY());
		}

		// Test getting Mouse delta position value
		TEST_METHOD(GetMouseDelta)
		{
			// Arrange
			Input::CreateManager();

			double xpos = 0.0;
			double ypos = 0.0;
			double xpos2 = 10.0;
			double ypos2 = 10.0;
			Input::getInstance()._mouse_callback(xpos, ypos);
			Input::getInstance()._mouse_callback(xpos2, ypos2);
			glm::vec2 expected = glm::vec2(-10.0, 10.0);

			// Act
			glm::vec2 result = Input::getInstance().GetMouseDelta();

			// Assert
			Assert::IsTrue(expected == result);
		}

		// Test scrollback
		TEST_METHOD(ScrollCallback)
		{
			// Arrange
			Input::CreateManager();

			float xScrollOffset = 17.8;
			float yScrollOffset = 18.4;

			// Act
			Input::getInstance()._scroll_callback(xScrollOffset, yScrollOffset);

			// Assert
			Assert::IsTrue(xScrollOffset == Input::getInstance().GetScrollOffsetX());
			Assert::IsTrue(yScrollOffset == Input::getInstance().GetScrollOffsetY());
		}

		// Test scrollback x value
		TEST_METHOD(GetScrollOffsetX)
		{
			// Arrange
			Input::CreateManager();

			// Act
			Input::getInstance()._scroll_callback(4.0, 4.5);

			// Assert
			Assert::IsTrue(4.0 == Input::getInstance().GetScrollOffsetX());
		}

		// Test scrollback y value
		TEST_METHOD(GetScrollOffsetY)
		{
			// Arrange
			Input::CreateManager();               

			// Act
			Input::getInstance()._scroll_callback(4.0, 4.5);

			// Assert
			Assert::IsTrue(4.5 == Input::getInstance().GetScrollOffsetY());
		}

		// Test mouse button down
		TEST_METHOD(GetMouseButtonDown)
		{
			// Arrange
			Input::CreateManager();

			// Act and Assert
			Assert::IsTrue(false == Input::getInstance().GetMouseButtonDown(1));
		}

		// Test mouse button
		TEST_METHOD(GetMouseButton)
		{
			// Arrange
			Input::CreateManager();

			// Act and Assert
			Assert::IsTrue(false == Input::getInstance().GetMouseButton(1));
		}

		// Test mouse button up
		TEST_METHOD(GetMouseButtonUp)
		{
			// Arrange
			Input::CreateManager();

			// Act and Assert
			Assert::IsTrue(false == Input::getInstance().GetMouseButtonUp(1));
		}

		// Test key button down
		TEST_METHOD(GetKeyDown)
		{
			// Arrange
			Input::CreateManager();

			// Act and Assert
			Assert::IsTrue(false == Input::getInstance().GetKeyDown(1));
		}

		// Test key button
		TEST_METHOD(GetKey)
		{
			// Arrange
			Input::CreateManager();

			// Act and Assert
			Assert::IsTrue(false == Input::getInstance().GetKey(1));
		}

		// Test key button up
		TEST_METHOD(GetKeyUp)
		{
			// Arrange
			Input::CreateManager();

			// Act and Assert
			Assert::IsTrue(false == Input::getInstance().GetKeyUp(1));
		}
	};
}