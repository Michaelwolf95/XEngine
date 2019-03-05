#include "LightComponent.h"
#include "GameObject.h"

LightComponent::LightComponent(glm::vec3 _color, float _intensity) : Light::Light()
{
	color = _color;
	intensity = _intensity;
}

void LightComponent::Start() {}
void LightComponent::Update() {}

glm::vec3 LightComponent::getLightColor()
{
	return color;
}

glm::vec3 LightComponent::getLightPos()
{
	return gameObject->transform->getPosition();
}

float LightComponent::getIntensity() {
	return intensity;
}
