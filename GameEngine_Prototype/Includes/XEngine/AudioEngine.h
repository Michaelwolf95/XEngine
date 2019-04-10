#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>

#include "GameObject.h" 

using namespace std;

//struct Vector3
//{
//	float x;
//	float y;
//	float z;
//	Vector3(float _x = 0.0f, float _y= 0.0f, float _z = 0.0f)
//	{
//		x = _x;
//		y = _y;
//		z = _z;
//	}
//};

struct Implementation {
	Implementation();
	~Implementation();

	void Update();

	FMOD::Studio::System* mpStudioSystem;
	FMOD::System* mpSystem;


	int mnNextChannelId;

	typedef map<string, FMOD::Sound*> SoundMap;
	typedef map<int, FMOD::Channel*> ChannelMap;
	typedef map<string, FMOD::Studio::EventInstance*> EventMap;
	typedef map<string, FMOD::Studio::Bank*> BankMap;
	BankMap mBanks;
	EventMap mEvents;
	SoundMap mSounds;
	ChannelMap mChannels;
};

class CAudioEngine
{
public:
	static void Init();
	static void Update();
	static void Shutdown();
	static int ErrorCheck(FMOD_RESULT result);
	FMOD::Channel* aChannel;// for pausing channel

	void LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
	void LoadEvent(const std::string& strEventName);
	void LoadSound(const string &strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false);
	void UnLoadSound(const string &strSoundName);
	void Set3dListenerAndOrientation(const glm::vec3& vPosition, const glm::vec3& vLook, const glm::vec3& vUp);
	void Set3dListenerAndOrientation(const FMOD_VECTOR& vPosition, const FMOD_VECTOR & vVel, const FMOD_VECTOR& vLook, const FMOD_VECTOR& vUp);
	//void UpdateListenerOrientation()
	//void Set3dListenerAndOrientation();
	int PlaySounds(const string &strSoundName, const glm::vec3& vPos = glm::vec3{ 0, 0, 0 }, float fVolumedB = 0.0f);
	void PlayEvent(const string &strEventName);
	void StopChannel(int nChannelId);
	void StopEvent(const string &strEventName, bool bImmediate = false);
	void GetEventParameter(const string &strEventName, const string &strEventParameter, float* parameter);
	void SetEventParameter(const string &strEventName, const string &strParameterName, float fValue);
	void StopAllChannels();
	void SetChannel3dPosition(int nChannelId, const glm::vec3& vPosition);
	void SetChannelVolume(int nChannelId, float fVolumedB);
	void Pause();
	void UnPause();
	bool IsPlaying(int nChannelId) const;
	bool IsEventPlaying(const string &strEventName) const;
	float dbToVolume(float dB);
	float VolumeTodB(float volume);
	FMOD_VECTOR VectorToFmod(const glm::vec3& vPosition);

};

#endif