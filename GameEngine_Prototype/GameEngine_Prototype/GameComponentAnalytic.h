#pragma once
#include "Component.h"
#include "Analytics.h"
class GameComponentAnalytic: public Component
{
public:
	int SAMPLE_SIZE = 10; //used for FPS sampling demo
	static int objectNum;
	static CSVMaker csv;

	GameComponentAnalytic();
	GameComponentAnalytic(int num);
	~GameComponentAnalytic();

	void Start() override;
	void Update() override;

	inline void fpsSample(int & sample_sz);
	inline void PrintFPS();
};

