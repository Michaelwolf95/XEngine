#include "GlobalLightComponent.h"
#include "GameObject.h"

GlobalLightComponent::GlobalLightComponent(glm::vec3 _color, float _intensity, glm::vec3 _direction)
	: LightComponent(_color, _intensity, TYPE_ID)
{
	direction = _direction;
}

void GlobalLightComponent::Start() {}
void GlobalLightComponent::Update() {}

glm::vec3 GlobalLightComponent::getLightColor()
{
	return color;
}

glm::vec3 GlobalLightComponent::getLightPos()
{
	return this->gameObject->transform->getPosition();
}

glm::vec3 GlobalLightComponent::getDirection()
{
	return direction;
}



