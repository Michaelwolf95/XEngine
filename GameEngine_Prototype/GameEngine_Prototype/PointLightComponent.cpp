#include "PointLightComponent.h"
#include "GameObject.h"
//class GameObject;

REGISTER_COMPONENT(PointLightComponent, "PointLightComponent")

PointLightComponent::PointLightComponent(glm::vec3 _color, float _intensity) 
	: LightComponent::LightComponent(_color, _intensity, TYPE_ID)
{
	//color = _color;
	//intensity = _intensity;
	//gameObject = gameObject;
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
