#include "PointLightComponent.h"
#include "GameObject.h"
//class GameObject;

REGISTER_COMPONENT(PointLightComponent, "PointLightComponent")

PointLightComponent::PointLightComponent(glm::vec3 _color, float _intensity, 
	float _constant, float _linear, float _quadratic) : LightComponent::LightComponent(_color, _intensity)
{
	UNIFORM_NAME = "pointLights";
	TYPE = LightType::PointLight;
	constant = _constant;
	linear = _linear;
	quadratic = _quadratic;
}


PointLightComponent::~PointLightComponent() {}

void PointLightComponent::Start() {}
void PointLightComponent::Update() {}

//float PointLightComponent::getConstant()
//{
//	return constant;
//}
//
//float PointLightComponent::getLinear()
//{
//	return linear;
//}
//
//float PointLightComponent::getQuadratic()
//{
//	return quadratic;
//}
//
//glm::vec3 PointLightComponent::getDirection() // can we take this out?
//{
//	return glm::vec3();  // not used
//}
//
//
//int PointLightComponent::getTypeID()
//{
//	return TYPE_ID;
//}

void PointLightComponent::Draw(Shader * shader, int &counter)
{
	shader->setVec3(UNIFORM_NAME + '[' + std::to_string(counter) + "].position", getLightPos());
	shader->setFloat(UNIFORM_NAME + '[' + std::to_string(counter) + "]." + VAR_NAME(constant), constant);
	shader->setFloat(UNIFORM_NAME + '[' + std::to_string(counter) + "]." + VAR_NAME(linear), linear);
	shader->setFloat(UNIFORM_NAME + '[' + std::to_string(counter) + "]." + VAR_NAME(quadratic), quadratic);
	LightComponent::Draw(shader, counter);
}
