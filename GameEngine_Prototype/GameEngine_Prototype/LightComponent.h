#pragma once
#include "Component.h"
#include "Light.h"
#include <glm/glm.hpp>
class LightComponent : public Component, public Light
{
public:
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	float intensity = 1.0f;
	LightComponent(glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f), float _intensity = 1.0f);
	void Start() override;
	void Update() override;
	glm::vec3 getLightColor() override;
	glm::vec3 getLightPos() override;
};

