#pragma once
#include "Component.h"
#include "Light.h"
#include "LightComponent.h"
#include <glm/glm.hpp>
class PointLightComponent : public LightComponent
{
public:
	static const int TYPE_ID = 1;
	//glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	//float intensity = 1.0f;
	// position is a function of transform component
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;

	PointLightComponent(glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f), float _intensity = 1.0f);
	void Start() override;
	void Update() override;
	glm::vec3 getLightColor() override;
	glm::vec3 getLightPos() override;
	float getConstant();
	float getLinear();
	float getQuadratic();
};

