#pragma once
#include "Component.h"
#include <glm/glm.hpp>



class ENGINE_API Emitter : public Component
{
public:
	Emitter();
	~Emitter();
	GameObject *gameObject;
	float velocity = 1.0f;
};