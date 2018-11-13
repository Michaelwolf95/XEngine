#pragma once
#include "Singleton.h"
// TimeManager
class Time : public Singleton<Time>
{
	friend class Singleton<Time>;
public:
	static float deltaTime;
	static float currentTime;
	float timeLastFrame;
	Time();
	static Time* CreateManager();
	void Init();
	void UpdateTime();

};

