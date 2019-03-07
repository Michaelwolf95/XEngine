#include "LightComponent.h"
#include "GameObject.h"

LightComponent::LightComponent(glm::vec3 _color, float _intensity, int typeID) : Light::Light()
{
	color = _color;
	intensity = _intensity;
	TYPE_ID = typeID;
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

glm::vec3 LightComponent::getAmbient()
{
	return ambient;
}

glm::vec3 LightComponent::getDiffuse()
{
	return diffuse;
}

glm::vec3 LightComponent::getSpecular()
{
	return specular;
}

float LightComponent::getConstant()
{
	return 0.0f; // does nothing
}

float LightComponent::getLinear()
{
	return 0.0f; // does nothing
}

float LightComponent::getQuadratic()
{
	return 0.0f; // does nothing
}

glm::vec3 LightComponent::getDirection()
{
	return glm::vec3(); // does nothing
}

int LightComponent::getTypeID()
{
	return TYPE_ID;
}
