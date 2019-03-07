#pragma once
#include "Component.h"
#include "Light.h"
#include <glm/glm.hpp>
class LightComponent : public Component, public Light
{
public:
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	float intensity = 1.0f;
	int TYPE_ID;
	
	glm::vec3 ambient = glm::vec3(0.05f);
	glm::vec3 diffuse = glm::vec3(0.8f);
	glm::vec3 specular = glm::vec3(1.0f);

	//GameObject *gameObject;
	LightComponent(glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f), float _intensity = 20.0f, int typeID = 0);
	virtual void Start() override;
	virtual void Update() override;
	glm::vec3 getLightColor() override;
	glm::vec3 getLightPos() override;
	float getIntensity() override;
	glm::vec3 getAmbient() override;
	glm::vec3 getDiffuse() override;
	glm::vec3 getSpecular() override;
	virtual float getConstant() override;
	virtual float getLinear() override;
	virtual float getQuadratic() override;
	virtual glm::vec3 getDirection() override;
	virtual int getTypeID() override = 0;
};

