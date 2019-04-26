#pragma once
#include "LibraryExport.h"
#include "Singleton.h"
namespace XEngine
{
	// TimeManager
	class ENGINE_API GameTime : public Singleton<GameTime> // TODO: Separate FPS into own class. 
	{
		struct FPS { //TODO: forward declaration?
			void(*getFPSCallback)(float fps) = nullptr;
			int counter = 0;
			int sampleSetSize = 60;
			float deltaTimeSum = 0.0f;
			bool isCounting = false;

			inline void doFPSCallback() {
				if (getFPSCallback != nullptr)
				{
					getFPSCallback(sampleSetSize / deltaTimeSum);
					getFPSCallback = nullptr;
				}
			}

			inline void getFPS(void(*callback)(float)) {
				if (counter == 0)
					deltaTimeSum = GameTime::deltaTime;
				else deltaTimeSum += GameTime::deltaTime;
				if (!(++counter < sampleSetSize))
				{
					if (callback != nullptr)
						getFPSCallback = callback;
					counter = 0;
				}
			}

			// Use this function to modify sample_sz to correct value.
			// Example: modSampleSize(sample_size)
			// Adjusts sample size by reference.
			void modSampleSize(int &sample_sz) {
				sample_sz *= sampleSetSize;
			}
		} fps;
		friend class Singleton<GameTime>;
	public:
		//FPS fps;
		static float deltaTime; // TODO: Create static functions
		static float currentTime; // TODO: Create static functions
		float timeLastFrame;
		GameTime();
		static GameTime* CreateManager();
		void Init();
		inline void updateTime();
		static void UpdateTime();
		inline void getFPS(void(*callback)(float));
		static void GetFPS(void(*callback)(float));
		inline void toggleFPS();
		static void ToggleFPS();
		static void ModSampleSize(int &sample_sz);
		static bool IsCounting();
		static void SetSampleSetSize(int sample_set_sz);
		//TODO: Move private functions to private
	};
}