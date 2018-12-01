#pragma once
#include "Singleton.h"
// TimeManager
class Time : public Singleton<Time>
{
	struct FPS { //TODO: forward declaration?
		void(*getFPSCallback)(float fps) = nullptr;
		int counter = 0;
		int mod = 60;
		float deltaTimeSum = 0.0f;
		bool isCounting = false;
		inline void doFPSCallback()	{
			if (getFPSCallback != nullptr)
			{
				getFPSCallback(mod / deltaTimeSum);
				//isCounting = false;
				getFPSCallback = nullptr;
			}
		}
		inline void getFPS(void(*callback)(float)) {
			if (counter == 0)
				deltaTimeSum = Time::deltaTime;
			else deltaTimeSum += Time::deltaTime;
			if (!(++counter < mod))
			{
				if (callback != nullptr)
					getFPSCallback = callback;
				counter = 0;
			}
		};
	};
	friend class Singleton<Time>;
public:
	FPS fps;
	static float deltaTime; // TODO: Create static functions
	static float currentTime; // TODO: Create static functions
	float timeLastFrame;
	Time();
	static Time* CreateManager();
	void Init();
	inline void updateTime();
	static void UpdateTime();
	inline void getFPS(void(*callback)(float));
	static void GetFPS(void(*callback)(float));
	inline void toggleFPS();
	static void ToggleFPS();
};

