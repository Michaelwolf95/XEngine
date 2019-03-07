#include "GlobalLightComponent.h"
#include "GameObject.h"

GlobalLightComponent::GlobalLightComponent(glm::vec3 _color, float _intensity, glm::vec3 _direction)
	: LightComponent(_color, _intensity, TYPE_ID)
{
	direction = _direction;
}

void GlobalLightComponent::Start() {}
void GlobalLightComponent::Update() {}

float GlobalLightComponent::getConstant()
{
	return 0.0f; // not used
}

float GlobalLightComponent::getLinear()
{
	return 0.0f; // not used
}

float GlobalLightComponent::getQuadratic()
{
	return 0.0f; // not used
}

glm::vec3 GlobalLightComponent::getDirection()
{
	return direction;
}

int GlobalLightComponent::getTypeID()
{
	return TYPE_ID;
}



