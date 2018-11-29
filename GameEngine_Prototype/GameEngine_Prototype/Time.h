#pragma once
#include "Singleton.h"
// TimeManager
class Time : public Singleton<Time>
{
	friend class Singleton<Time>;
public:
	int counter = 0;
	int mod = 500;
	float deltaTimeSumOf60 = 0.0f;
	bool isCounting = false;
	static float deltaTime;
	static float currentTime;
	float timeLastFrame;
	Time();
	static Time* CreateManager();
	void Init();
	void UpdateTime();
	void getFPS(void(*callback)(float));
	static void GetFPS(void(*callback)(float));
	void toggleFPS();
	static void ToggleFPS();

};

