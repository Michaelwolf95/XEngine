#pragma once
#include "Component.h"
#include "Light.h"
#include "LightComponent.h"
#include <glm/glm.hpp>
class PointLightComponent : public LightComponent
{
public:
	static const int TYPE_ID = 1;

	// position is a function of transform component
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;

	PointLightComponent(glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f), 
		float _intensity = 1.0f);

	void Start() override;
	void Update() override;

	float getConstant() override;
	float getLinear() override;
	float getQuadratic() override;
	glm::vec3 getDirection() override;
	int getTypeID() override;
};