#include "FPSText.h"
#include <iostream>

REGISTER_COMPONENT(FPSText, "FPSText")

Text* tempFPS = nullptr;

FPSText::FPSText()
{
	this->executeInEditMode = true;
}

FPSText::~FPSText()
{
	tempFPS = nullptr;
	fpsText = nullptr;
}

void FPSText::Start()	{}

void UpdateFPS(float fps)
{
	tempFPS->setText(std::to_string((int)fps));
}

void FPSText::Update()	
{
	XEngine::GameTime::GetFPS(UpdateFPS);
}

void FPSText::OnEnable() 
{
	this->ConnectToText();
}

void FPSText::ConnectToText()
{
	fpsText = nullptr;

	if (this->gameObject->FindComponent(typeid(Text), (void**)&fpsText))
	{
		std::cout << "FPS Text has connected." << std::endl;
		tempFPS = (Text*)((char*)fpsText - 8);
	}
	else
	{
		std::cout << "FPS Text failed to connect." << std::endl;
	}
}