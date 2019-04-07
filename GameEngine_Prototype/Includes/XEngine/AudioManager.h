#pragma once
#include "Singleton.h"
#include "AudioEngine.h"

//creating the only one Implementation for all audio to use
class AudioManager : public Singleton<AudioManager>
{
	friend class Singleton<AudioManager>;
public:
	static AudioManager* CreateManager();
	AudioManager();
	~AudioManager();
	void Init();
	void UpdateAudio();
	CAudioEngine sound;
};

