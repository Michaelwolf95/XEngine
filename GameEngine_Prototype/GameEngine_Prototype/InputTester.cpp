#include "InputTester.h"
#include "GameObject.h"
#include "Time.h"
//#include <GLFW/glfw3.h>
#include "ApplicationManager.h"
#include "Input.h"
#include <iostream>
#include <fstream>
#include <stdio.h>

std::ofstream csv;

InputTester::InputTester()
{
	csv.open("fpstest.csv", std::ofstream::app);
	rotationSpeed = 0.0f;
}


InputTester::~InputTester()
{
	std::cout << "~InputTester" << std::endl;
	csv.close();

}
void printFPS(float fps)
{
	std::cout << fps << std::endl;
	csv << fps << std::endl;
	//Time::getInstance().GetFPS(printFPS);
}

void InputTester::Start()
{
}

void InputTester::Update()
{
	float deltaY = rotationSpeed * Time::deltaTime;
	float deltaX = deltaY;
	float horizontal = 0.f;
	float vertical = 0.0f;
	
	if (Input::getInstance().GetKey(GLFW_KEY_LEFT))	horizontal = 1;
	if (Input::getInstance().GetKey(GLFW_KEY_RIGHT)) horizontal = -1;
	if (Input::GetKey(GLFW_KEY_UP)) vertical = 1;
	if (Input::GetKey(GLFW_KEY_DOWN)) vertical = -1;
	if (Input::GetKeyDown(GLFW_KEY_EQUAL)) rotationSpeed += 10.0f;
	if (Input::GetKeyDown(GLFW_KEY_MINUS)) rotationSpeed -= 10.0f;
	if (Input::GetKeyDown(GLFW_KEY_P)) Input::ToggleCursor();
	//if (Input::GetKeyDown(GLFW_KEY_C)) 
	if (Input::GetKeyDown(GLFW_KEY_F)) Time::ToggleFPS();
	if (Input::GetKeyDown(GLFW_KEY_R))	sampleSize = 10;
	if (sampleSize)
	{
		Time::GetFPS(printFPS);
		--sampleSize;
	}

	deltaY *= horizontal;
	deltaX *= vertical;
	gameObject->transform->Rotate(glm::vec3(deltaX, deltaY, 0.f));
}

