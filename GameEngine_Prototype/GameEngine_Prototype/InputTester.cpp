#include "InputTester.h"
#include "GameObject.h"
#include "Time.h"
//#include <GLFW/glfw3.h>
#include "ApplicationManager.h"
#include "Input.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "FreeLookCameraController.h"

std::ofstream csv;

InputTester::InputTester()
{
	csv.open("fpstest.csv");
	rotationSpeed = 0.0f;
}


InputTester::~InputTester()
{
	std::cout << "~InputTester" << std::endl;
	csv.close();

}
void logFPS(float fps)
{
	csv << fps << std::endl;
}

void printFPS(float fps)
{
	std::cout << fps << std::endl;
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
	if (Input::GetKeyDown(GLFW_KEY_R)) Time::ModSampleSize(SAMPLE_SIZE = 25);

	fpsSample(SAMPLE_SIZE);
	PrintFPS();

	deltaY *= horizontal;
	deltaX *= vertical;
	gameObject->transform->Rotate(glm::vec3(deltaX, deltaY, 0.f));
	//(gameObject + sizeof(GameObject))->transform->LookAt(glm::vec3(0.5f, 1.f, 0.f), glm::vec3(0.0f, 1.0f, 0.0f));
	//std::cin.get();
	gameObject->transform->Translate(glm::vec3(Input::GetDeltaPosX() * Time::deltaTime, 0.0f, Input::GetDeltaPosY() * Time::deltaTime));
	//std::cout << Input::GetDeltaPosX() << std::endl;
	//gameObject[1].transform->setLocalRotationEuler(sin(deltaX), cos(deltaY), 0.f);
}

void InputTester::fpsSample(int &sample_sz)
{
	if (sample_sz > 0)
	{
		--sample_sz;
		Time::GetFPS(logFPS);
	}
}

void InputTester::PrintFPS()
{
	if (Time::getInstance().fps.isCounting)
		Time::GetFPS(printFPS);
}