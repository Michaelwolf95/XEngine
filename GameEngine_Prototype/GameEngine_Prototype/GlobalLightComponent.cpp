#include "GlobalLightComponent.h"
#include "GameObject.h"

REGISTER_COMPONENT(GlobalLightComponent, "GlobalLightComponent")

GlobalLightComponent::GlobalLightComponent(glm::vec3 _color, float _intensity, glm::vec3 _initial_direction)
	: LightComponent(_color, _intensity, TYPE_ID)
{
	direction = _initial_direction;
	//direction = this->gameObject->transform->getForwardDirection();
	UNIFORM_NAME = "globalLights";
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

void GlobalLightComponent::draw(Shader * shader, int &counter)
{
	direction = this->gameObject->transform->getForwardDirection(); // TODO: Only update when necessary (static techique)
	shader->setVec3(UNIFORM_NAME + '[' + std::to_string(counter) + "]." + VAR_NAME(direction), direction);
	LightComponent::draw(shader, counter);
}