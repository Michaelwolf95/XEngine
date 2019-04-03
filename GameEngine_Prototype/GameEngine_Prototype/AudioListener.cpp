#include "AudioListener.h"
#include "AudioManager.h"
#include "AudioEngine.h"

REGISTER_COMPONENT(AudioListener, "AudioListener")

void AudioListener::Start()
{
	AudioManager::getInstance().sound.Set3dListenerAndOrientation(pos, velocity, forward, up);

}

void AudioListener::Update()
{
	// MOVE THIS TO AUDIO LISTENER
	/*glm::mat4 camView = RenderManager::getInstance().getCurrentCamera()->getView();
	glm::vec3 camLocation(camView[3].x, camView[3].y, camView[3].z);*/
	//AudioManager::getInstance().sound.Set3dListenerAndOrientation(camLocation, camLocation, camLocation);

	glm::vec3 p = this->gameObject->transform->getPosition();
	pos.x = p.x;
	pos.y = p.y;
	pos.z = p.z;
	glm::vec3 f = this->gameObject->transform->getForwardDirection();
	forward.x = f.x;
	forward.y = f.y;
	forward.z = f.z;
	glm::vec3 u = this->gameObject->transform->getUpDirection();
	up.x = u.x;
	up.y = u.y;
	up.z = u.z;
}

AudioListener::AudioListener()
{
}


AudioListener::~AudioListener()
{
}
