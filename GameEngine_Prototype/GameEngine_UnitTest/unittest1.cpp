#include "stdafx.h"
#include "CppUnitTest.h"
//
#include "XEngine.h"
using namespace XEngine;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace XEngine_UnitTest
{	
	/*
		Classes not tested:
			- AssetManager
			- DebugUtility
			- Singleton
	*/

	TEST_CLASS(InputManager)
	{
	public:
		/*
			Functions/Methods not tested:
				- CreateManager()
				- Init()
				- UpdateInput()
				- _mouse_button_callback(int button, int action, int mod)
				- INPUT_MOUSE_CALLBACK(GLFWwindow* window, double xpos, double ypos)
				- INPUT_SCROLL_CALLBACK(GLFWwindow* window, double xoffset, double yoffset)
				- INPUT_MOUSE_BUTTON_CALLBACK(GLFWwindow * window, int button, int action, int mods)
				- ShowCursor(bool enable)
				- ToggleCursor()
				- checkKeyInputs()
				- validateMouseInputValue(int flfw_mouse_button)
				- validateKeyInputValue(int glfw_key)
		*/

		TEST_METHOD(GetMousePosX)
		{
			Input::CreateManager();

			Assert::AreEqual(0.0, Input::getInstance().GetMousePosX());
		}

		TEST_METHOD(GetMousePosY)
		{
			Input::CreateManager();

			Assert::AreEqual(0.0, Input::getInstance().GetMousePosY());
		}
		/*
		TEST_METHOD(MouseCallback)
		{
			Input::CreateManager();

			double xpos = 12.0;
			double ypos = 15.0;

			Input::getInstance()._mouse_callback(xpos, ypos);

			//Why does GetMousePosX and GetMousePosY return double but the method it calls returns bool?
			Assert::AreEqual(xpos, Input::getInstance().GetMousePosX());
			Assert::AreEqual(ypos, Input::getInstance().GetMousePosY());
		}
		*/
		TEST_METHOD(GetMousePos)
		{
			Input::CreateManager();

			double xpos = 13.5;
			double ypos = 18.5;
			glm::vec2 mousePos = glm::vec2(xpos, ypos);

			Input::getInstance()._mouse_callback(xpos, ypos);

			Assert::IsTrue(mousePos == Input::getInstance().GetMousePos());
		}

		TEST_METHOD(GetDeltaPosX)
		{
			Input::CreateManager();

			double xpos = 11.0;
			double ypos = 12.5;

			Input::getInstance()._mouse_callback(xpos, ypos);

			Assert::AreEqual(2.5, Input::getInstance().GetDeltaPosX());
		}

		TEST_METHOD(GetDeltaPosY)
		{
			Input::CreateManager();

			double xpos = 11.0;
			double ypos = 12.5;

			Input::getInstance()._mouse_callback(xpos, ypos);

			Assert::AreEqual(-6.0, Input::getInstance().GetDeltaPosY());
		}

		TEST_METHOD(GetMouseDelta)
		{
			Input::CreateManager();

			double xpos = 1.0;
			double ypos = 2.5;
			glm::vec2 mouseDelta = glm::vec2(10.0, -10.0);

			Input::getInstance()._mouse_callback(xpos, ypos);

			Assert::IsTrue(mouseDelta == Input::getInstance().GetMouseDelta());
		}

		TEST_METHOD(ScrollCallback)
		{
			Input::CreateManager();

			double xScrollOffset = 17.8;
			double yScrollOffset = 18.4;

			Input::getInstance()._scroll_callback(xScrollOffset, yScrollOffset);

			Assert::AreEqual(xScrollOffset, Input::getInstance().GetScrollOffsetX());
			Assert::AreEqual(yScrollOffset, Input::getInstance().GetScrollOffsetY());
		}

		TEST_METHOD(GetScrollOffsetX)
		{
			Input::CreateManager();

			Input::getInstance()._scroll_callback(4.0, 4.5);

			Assert::AreEqual(4.0, Input::getInstance().GetScrollOffsetX());
		}

		TEST_METHOD(GetScrollOffsetY)
		{
			Input::CreateManager();               

			Input::getInstance()._scroll_callback(4.0, 4.5);

			Assert::AreEqual(4.5, Input::getInstance().GetScrollOffsetY());
		}

		TEST_METHOD(GetMouseButtonDown)
		{
			Input::CreateManager();

			Assert::IsTrue(false == Input::getInstance().GetMouseButtonDown(1));
		}

		TEST_METHOD(GetMouseButton)
		{
			Input::CreateManager();

			Assert::IsTrue(false == Input::getInstance().GetMouseButton(1));
		}

		TEST_METHOD(GetMouseButtonUp)
		{
			Input::CreateManager();

			Assert::IsTrue(false == Input::getInstance().GetMouseButtonUp(1));
		}

		TEST_METHOD(GetKeyDown)
		{
			Input::CreateManager();

			Assert::IsTrue(false == Input::getInstance().GetKeyDown(1));
		}

		TEST_METHOD(GetKey)
		{
			Input::CreateManager();

			Assert::IsTrue(false == Input::getInstance().GetKey(1));
		}

		TEST_METHOD(GetKeyUp)
		{
			Input::CreateManager();

			Assert::IsTrue(false == Input::getInstance().GetKeyUp(1));
		}
		
		TEST_METHOD(Test_Test)
		{
			int num = 1;

			Assert::IsTrue(num == 1);
		}
	};
	
	TEST_CLASS(TimeManager)
	{
	public:
		/*
			Functions/Methods not tested:
				- CreateManager()
				- Init()
				- UpdateInput()
				- ModSampleSize(int &sample_sz)
				- SetSampleSetSize(int sample_set_sz)
				- GetFPS(void(*callback)(float))
		*/

		TEST_METHOD(UpdateTime)
		{
			GameTime::CreateManager();

			float current = GameTime::getInstance().currentTime;
			float delta = current - GameTime::getInstance().timeLastFrame;
			float last = current;

			GameTime::getInstance().UpdateTime();

			Assert::AreEqual(GameTime::getInstance().currentTime, current);
			Assert::AreEqual(GameTime::getInstance().deltaTime, delta);
			Assert::AreEqual(GameTime::getInstance().timeLastFrame, last);
		}

		TEST_METHOD(ToggleFPS)
		{
			GameTime::CreateManager();

			GameTime::getInstance().ToggleFPS();

			Assert::IsTrue(true == GameTime::getInstance().IsCounting());
		}

		TEST_METHOD(IsCounting)
		{
			GameTime::CreateManager();

			Assert::IsTrue(true == GameTime::getInstance().IsCounting());
		}
	};
}