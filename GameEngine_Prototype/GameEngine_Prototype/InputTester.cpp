#include "InputTester.h"
#include "GameObject.h"
#include "Time.h"
#include "ApplicationManager.h"
#include "Input.h"
#include <iostream>
#include "FreeLookCameraController.h"
using namespace XEngine;

//std::ofstream csv_test;
CSVMaker InputTester::csv_test = CSVMaker("fps_sample"); // *** if static must initialize outside of class's body

InputTester::InputTester()
{
	rotationSpeed = 0.0f;
}

InputTester::~InputTester()
{
	csv_test.Close(); // *** close the file when application is closed.
}

static int log_counter = 1; // *** used for object number in csv file.
void logFPS(float fps) // *** function for Time class to callback to.
{
	InputTester::csv_test.Write(log_counter++, fps); // *** notice that I had to make CSVMaker object static (maybe there is a better way?)
}

void InputTester::printFPS(float fps)
{
	std::cout << fps << std::endl;
}

void InputTester::Start()
{
	// not sure what to do with this function
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
	///if (Input::GetKeyDown(GLFW_KEY_P)) Input::ToggleCursor(); // Somebody added into another class
	///if (Input::GetKeyDown(GLFW_KEY_C)) 
	if (Input::GetKeyDown(GLFW_KEY_F)) Time::ToggleFPS(); // ** Do not use for sampling -- use below method instead
	if (Input::GetKeyDown(GLFW_KEY_R)) {
		int modSampleSize = 1; // **number of samples per FPS sample
		Time::SetSampleSetSize(modSampleSize); // **set the number of samples per FPS sample. 
		Time::ModSampleSize(SAMPLE_SIZE_TEST = 25); // **sets sample size to 25 (how many measurements to be done). sample size is then modified to size necessary to take averaged samples.
	}
	fpsSample(SAMPLE_SIZE_TEST); // **modified sample size is passed to custom function -- maybe I should integrate this into FPS struct...
	PrintFPS(); // **indefinitely prints FPS to console if isPrinting is true. Separate feature from fps sampling. Do not use if you only want to log fps samples to file.

	deltaY *= horizontal;
	deltaX *= vertical;
	gameObject->transform->Rotate(glm::vec3(deltaX, deltaY, 0.f));
	///(gameObject + sizeof(GameObject))->transform->LookAt(glm::vec3(0.5f, 1.f, 0.f), glm::vec3(0.0f, 1.0f, 0.0f));
	gameObject->transform->Translate(glm::vec3(Input::GetDeltaPosX() * Time::deltaTime, 0.0f, Input::GetDeltaPosY() * Time::deltaTime));
	///std::cout << Input::GetDeltaPosX() << std::endl;
	///gameObject[1].transform->setLocalRotationEuler(sin(deltaX), cos(deltaY), 0.f);
}

// **Example of taking limited samples onto a logger file
void InputTester::fpsSample(int &sample_sz)
{
	if (sample_sz > 0)
	{
		--sample_sz;
		Time::GetFPS(logFPS); // **passes logFPS function as callback function.
	}
}

// **Example of printing to console -- ultimately we may use it to send values to renderer for onscreen display.
void InputTester::PrintFPS()
{
	if (Time::getInstance().IsCounting()) // **use IsCounting function to find out if FPS system is running.
		Time::GetFPS(printFPS); // **passes printFPS function as callback function.
}