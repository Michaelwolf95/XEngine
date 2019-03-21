#include "AudioEngine.h"
#include "AudioComponent.h"
#include "AudioManager.h"
#include "GameObject.h" 
#include "Component.h"
#include <math.h> 
#include "RenderManager.h"
//CAudioEngine test;
//soundTest->Init();
//
////LoadSound(const std::string& strSoundName, bool b3d, bool bLooping, bool bStream)
////parameters about streaming, looping, and whether or not it's a 3D sound
//soundTest->LoadSound(soundPath1, false, true, true);
//soundTest->PlaySounds(soundPath1);

REGISTER_COMPONENT(AudioComponent, "AudioComponent")

void AudioComponent::Start() {
	string soundPath1 = "../Assets/sounds/inception_sound.mp3";
	Load3D(soundPath1, true, true, true);
	glm::mat4 camView = RenderManager::getInstance().getCurrentCamera()->getView();
	Vector3 camLocation(camView[3].x, camView[3].y, camView[3].z);
	Play(soundPath1, camLocation, 1);
}

void AudioComponent::Update()
{
	// get our pos
	// get obj pos
	// perform distance formula
	//  get it\in relation to the range 

	//
}

AudioComponent::AudioComponent()
{
	//string soundPath1 = "../Assets/sounds/inception_sound.mp3";
	//std::shared_ptr<AudioComponent> soundTest(new AudioComponent());
	//Load3D(soundPath1, true, true, true);
	range = 1500;
}
AudioComponent::~AudioComponent()
{
	AudioManager::getInstance().sound.UnLoadSound("../Assets/sounds/inception_sound.mp3");
}

void AudioComponent::Load2D(string path, bool loop, bool stream)
{
	//CAudioEngine sound = AudioManager::getInstance();
	//AudioManager::AudioEngine;
	AudioManager::getInstance().sound.LoadSound(path, false, loop, stream);
	
	//AudioManager::getInstance().LoadSound(path, 3d, loop, stream);
}

void AudioComponent::Load3D(string path, bool location, bool loop, bool stream) {

	if (location == true) {
		AudioManager::getInstance().sound.LoadSound(path, location, loop, stream);
	}
}

//void AudioComponent::Distance(Vector3 cam) {
//	//---------------------------------------------
//	distance = sqrt(pow(cam.x - gameObject->transform->getPosition().x,2) + pow(cam.y - gameObject->transform->getPosition().y,2) + pow(cam.z - gameObject->transform->getPosition().z,2));
//
//
//	/*std::cout << sqrt(pow(x - x1, 2) + pow(y - y2, 2) + pow(z - z2, 2)) << "!\n";
//	std::cout << (sqrt(pow(x - x1, 2) + pow(y - y2, 2) + pow(z - z2, 2)) / 20);*/
//
//	//use the void SetVolume(float value) thats directly from the api like the function sets 
//
//	//under 1
//	float x = distance / range;
//	if (x < 1) { // no sound
//			//play sound
//	}
//	else {
//
//	}
//		
//}

void AudioComponent::Play(string path, Vector3 location, float fVolumedB) {
	AudioManager::getInstance().sound.PlaySounds(path, location, fVolumedB);
	//AudioManager::getInstance().sound.PlaySound
}



//void AudioComponent::Play3DSound(string path, ) {
//	//void PlaySound(const string& strSoundName, const Vector3& vPos = Vector3{ 0, 0, 0 }, float fVolumedB = 0.0f);
//	//PlaySound
//}

