#pragma once
#include "Component.h"
#include "Analytics.h"

class InputTester : public Component
{
public:
	float rotationSpeed;
	
	// ** analytics example
	static CSVMaker csv_test;
	int SAMPLE_SIZE_TEST; //used for FPS sampling demo
	
	static void printFPS(float fps);

	InputTester();
	~InputTester();
	void Start() override;
	void Update() override;
	inline void fpsSample(int &sample_sz);
	inline void PrintFPS();
};

