#include "AudioManager.h"
#include "AudioEngine.h"

AudioManager::AudioManager() {}
AudioManager::~AudioManager() {}

AudioManager* AudioManager::CreateManager()
{
	AudioManager* instance = &AudioManager::getInstance();

	instance->Init();

	return instance;
}

void AudioManager::Init()
{
	sound.Init();
	//CAudioEngine::Init();
}

void AudioManager::UpdateAudio() {
	sound.Update();
}