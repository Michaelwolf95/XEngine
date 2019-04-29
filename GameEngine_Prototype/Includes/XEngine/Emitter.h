#pragma once
#include "Component.h"
#include <glm/glm.hpp>



class ENGINE_API Emitter : public Component
{
public:
	Emitter();
	~Emitter();
	GameObject *objectToEmit;
	float velocity = 1.0f;
	glm::vec3 offset = glm::vec3(0.0f);
	float scaleFactor = 1.0f;
	// direction determined by direction of emitter
};