#pragma once
#include "Component.h"
class GameComponentAnalytic: public Component
{
public:
	int sampleSize = 0; //used for FPS sampling demo

	
	GameComponentAnalytic();
	~GameComponentAnalytic();
	void Start() override;
	void Update() override;
	inline void fpsSample(int & sample_sz);
	inline void PrintFPS();
};

