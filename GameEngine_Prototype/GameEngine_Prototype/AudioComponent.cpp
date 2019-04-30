#include "AudioEngine.h"
#include "AudioComponent.h"
#include "AudioManager.h"
#include "GameObject.h" 
#include "Component.h"
#include <math.h>
#include "RenderManager.h"
#include "AssetManager.h"
#include "imgui_inspector_extensions.h"
#include "imgui_stdlib.h"
#include "Input.h"

REGISTER_COMPONENT(AudioComponent, "AudioComponent")

void AudioComponent::Start() {
	//string soundPath1 = "../Assets/sounds/foghorn.wav"; 
	if (soundPath.size() > 0)
	{
		Load3D(soundPath, is3D, repeat, true);
	}
}

void AudioComponent::Update()
{
	// TESTING ONLY - REMOVE LATER
	if (Input::GetKeyDown(GLFW_KEY_SPACE))
	{
		Play();
		std::cout << "PLAY!" << std::endl;
	}
	if (Input::GetKeyDown(GLFW_KEY_P))
	{
		if (pauseToggle) {
			pauseToggle = false;
			Pause();
			std::cout << "Pause!" << std::endl;
		}
		else {
			pauseToggle = true;
			UnPause();
			std::cout << "UnPause!" << std::endl;
		}
	}
}

AudioComponent::AudioComponent()
{
}
AudioComponent::~AudioComponent()
{
	//AudioManager::getInstance().sound.UnLoadSound(soundPath);
}

void AudioComponent::Load3D(string path, bool location, bool loop, bool stream)
{
	AudioManager::getInstance().sound.LoadSound(path, location, loop, stream);
}

void AudioComponent::Play(string path, glm::vec3 location, float fVolumedB)
{
	//mChannel = AudioManager::getInstance().sound.PlaySounds(path, location, fVolumedB);
	AudioManager::getInstance().sound.PlaySounds(path, location, fVolumedB);
	//AudioManager::getInstance().sound.SetChannelVolume(mChannel, .1);
}

void AudioComponent::Play()
{
	glm::vec3 objectLocation = gameObject->transform->getPosition();
	Play(soundPath, objectLocation, 1);
}

void AudioComponent::Pause()
{
	AudioManager::getInstance().sound.Pause();
}
void AudioComponent::UnPause()
{
	AudioManager::getInstance().sound.UnPause();
}

void AudioComponent::DrawInspector()
{
	// File Path 
	ImGui::InputText("AudioClip", &soundPath);
	// File Path Drag and Drop
	const ImGuiPayload* payload = ImGui::GetDragDropPayload();
	if (payload != nullptr && payload->IsDataType("FILE_DRAG"))
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_DRAG"))
			{
				IM_ASSERT(payload->DataSize == 128);
				const char* payload_n = (const char*)payload->Data;

				std::string fileName(payload_n);
				if (fileName.substr(fileName.find_last_of(".")) == ".mp3" || fileName.substr(fileName.find_last_of(".")) == ".Mp3" || fileName.substr(fileName.find_last_of(".")) == ".wav")
				{
					std::cout << "Dropping SoundPath" << std::endl;
					if (fileName != soundPath)
					{
						// temporary fix, replace any backward slash with a forward slash
						std::replace(fileName.begin(), fileName.end(), '\\', '/');
						soundPath = fileName;
					}
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
	// Flags
	ImGui::Checkbox("Is 3D", &is3D);
	ImGui::SameLine();
	//add loop
	ImGui::Checkbox("Loop", &repeat);
}
