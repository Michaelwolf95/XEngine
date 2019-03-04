#pragma once
#include "Component.h"
#include "AudioManager.h"
class AudioComponent : public Component
{
public:
	void Start() override;
	void Update() override;
	float range, distance;

	AudioComponent();
	~AudioComponent();
	void Load2D(string, bool, bool, bool);
	void Load3D(string, bool, bool, bool);
	void Play(string, Vector3, float);
	void Distance(Vector3);

};