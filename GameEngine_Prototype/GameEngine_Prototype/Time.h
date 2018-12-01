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

		// Use this function to modify sample_sz to correct value.
		// Example: modSampleSize(sample_size)
		// Adjusts sample size by reference.
		void modSampleSize(int &sample_sz) {
			sample_sz *= mod;
		}
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
	static void ModSampleSize(int &sample_sz);
};

