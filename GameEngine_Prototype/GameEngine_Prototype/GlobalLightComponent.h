#pragma once
#include "Component.h"
#include "Light.h"
#include "LightComponent.h"
#include <glm/glm.hpp>
class GlobalLightComponent : public LightComponent
{
public:
	static const int TYPE_ID = 2;

	glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
	GlobalLightComponent(glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f), 
		float _intensity = 1.0f, 
		glm::vec3 _direction = glm::vec3(0.0f, -1.0f, 0.0f));

	void Start() override;
	void Update() override;

	float getConstant() override;
	float getLinear() override;
	float getQuadratic() override;
	glm::vec3 getDirection() override;
	int getTypeID() override;
};