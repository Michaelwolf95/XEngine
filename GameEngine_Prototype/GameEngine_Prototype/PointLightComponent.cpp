#include "PointLightComponent.h"
#include "GameObject.h"
//class GameObject;

REGISTER_COMPONENT(PointLightComponent, "PointLightComponent")

PointLightComponent::PointLightComponent(glm::vec3 _color, float _intensity) 
	: LightComponent::LightComponent(_color, _intensity, TYPE_ID)
{
	UNIFORM_NAME = "pointLights";
	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.032f;
}


PointLightComponent::~PointLightComponent() {}

void PointLightComponent::Start() {}
void PointLightComponent::Update() {}

float PointLightComponent::getConstant()
{
	return constant;
}

float PointLightComponent::getLinear()
{
	return linear;
}

float PointLightComponent::getQuadratic()
{
	return quadratic;
}

glm::vec3 PointLightComponent::getDirection() // can we take this out?
{
	return glm::vec3();  // not used
}


int PointLightComponent::getTypeID()
{
	return TYPE_ID;
}

void PointLightComponent::draw(Shader * shader, int &counter)
{
	shader->setVec3(UNIFORM_NAME + '[' + std::to_string(counter) + "].position", getLightPos());
	shader->setFloat(UNIFORM_NAME + '[' + std::to_string(counter) + "]." + VAR_NAME(constant), constant);
	shader->setFloat(UNIFORM_NAME + '[' + std::to_string(counter) + "]." + VAR_NAME(linear), linear);
	shader->setFloat(UNIFORM_NAME + '[' + std::to_string(counter) + "]." + VAR_NAME(quadratic), quadratic);
	LightComponent::draw(shader, counter);
}
