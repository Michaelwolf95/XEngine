#pragma once
#include "Component.h"

class InputTester : public Component
{
public:
	float rotationSpeed;
	int sampleSize = 0; //used for FPS sampling demo
	InputTester();
	~InputTester();
	void Start() override;
	void Update() override;
	inline void fpsSample(int & sample_sz);
	inline void PrintFPS();
};

