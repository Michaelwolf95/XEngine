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

			double xpos = 12.5;
			double ypos = 11.0;

			Input::getInstance()._mouse_callback(xpos, ypos);

			double xpos2 = 18.5;
			double ypos2 = 11.0;

			Input::getInstance()._mouse_callback(xpos2, ypos2);

			Assert::AreEqual(-6.0, Input::getInstance().GetDeltaPosX());
		}

		TEST_METHOD(GetDeltaPosY)
		{
			Input::CreateManager();

			double xpos = 11.0;
			double ypos = 12.5;

			Input::getInstance()._mouse_callback(xpos, ypos);

			double xpos2 = 11.0;
			double ypos2 = 18.5;

			Input::getInstance()._mouse_callback(xpos2, ypos2);

			Assert::AreEqual(6.0, Input::getInstance().GetDeltaPosY());
		}

		TEST_METHOD(GetMouseDelta)
		{
			Input::CreateManager();

			double xpos = 0.0;
			double ypos = 0.0;
			double xpos2 = 10.0;
			double ypos2 = 10.0;
			Input::getInstance()._mouse_callback(xpos, ypos);
			Input::getInstance()._mouse_callback(xpos2, ypos2);

			glm::vec2 expected = glm::vec2(-10.0, 10.0);
			glm::vec2 result = Input::getInstance().GetMouseDelta();
			Logger::WriteMessage(std::to_string(result.x).c_str());
			Logger::WriteMessage(std::to_string(result.y).c_str());
			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(ScrollCallback)
		{
			Input::CreateManager();

			float xScrollOffset = 17.8;
			float yScrollOffset = 18.4;

			Input::getInstance()._scroll_callback(xScrollOffset, yScrollOffset);

			Assert::IsTrue(xScrollOffset == Input::getInstance().GetScrollOffsetX());
			Assert::IsTrue(yScrollOffset == Input::getInstance().GetScrollOffsetY());
		}

		TEST_METHOD(GetScrollOffsetX)
		{
			Input::CreateManager();

			Input::getInstance()._scroll_callback(4.0, 4.5);

			Assert::IsTrue(4.0 == Input::getInstance().GetScrollOffsetX());
		}

		TEST_METHOD(GetScrollOffsetY)
		{
			Input::CreateManager();               

			Input::getInstance()._scroll_callback(4.0, 4.5);

			Assert::IsTrue(4.5 == Input::getInstance().GetScrollOffsetY());
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