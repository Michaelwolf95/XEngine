#include "PointLightComponent.h"
#include "GameObject.h"
//class GameObject;

PointLightComponent::PointLightComponent(glm::vec3 _color, float _intensity) 
	: LightComponent::LightComponent()
{
	color = _color;
	intensity = _intensity;
	//gameObject = kgameObject;

}

void PointLightComponent::Start() {}
void PointLightComponent::Update() {}

glm::vec3 PointLightComponent::getLightColor()
{
	return color;
}

glm::vec3 PointLightComponent::getLightPos()
{
	return this->gameObject->transform->getPosition();
}
