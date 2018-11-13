#pragma once
// TimeManager
class Time
{
public:
	static Time* instance;
	static float deltaTime;
	static float currentTime;
	float timeLastFrame;
	Time();
	~Time();
	static Time* CreateManager();
	void Init();
	void UpdateTime();

};

