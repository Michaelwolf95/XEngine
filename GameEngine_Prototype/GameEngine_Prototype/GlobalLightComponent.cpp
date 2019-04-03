#include "GlobalLightComponent.h"
#include "GameObject.h"

REGISTER_COMPONENT(GlobalLightComponent, "GlobalLightComponent")

GlobalLightComponent::GlobalLightComponent(glm::vec3 _color, float _intensity/*, glm::vec3 _initial_direction*/)
	: LightComponent(_color, _intensity)
{
	UNIFORM_NAME = "globalLights";
	TYPE = LightType::GlobalLight;
	//direction = _initial_direction;
	//direction = this->gameObject->transform->getForwardDirection();
}

void GlobalLightComponent::Start() {}
void GlobalLightComponent::Update() {}

//float GlobalLightComponent::getConstant()
//{
//	return 0.0f; // not used
//}
//
//float GlobalLightComponent::getLinear()
//{
//	return 0.0f; // not used
//}
//
//float GlobalLightComponent::getQuadratic()
//{
//	return 0.0f; // not used
//}
//
//glm::vec3 GlobalLightComponent::getDirection()
//{
//	return direction;
//}
//
//int GlobalLightComponent::getTypeID()
//{
//	return TYPE_ID;
//}

void GlobalLightComponent::Draw(Shader * shader, int &counter)
{
	shader->setVec3(UNIFORM_NAME + '[' + std::to_string(counter) + "].direction", getDirection()); // TODO: Only update when necessary (static techique)
	LightComponent::Draw(shader, counter);
}