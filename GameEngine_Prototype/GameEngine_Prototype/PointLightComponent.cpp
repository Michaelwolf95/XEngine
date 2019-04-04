#include "PointLightComponent.h"
#include "GameObject.h"
//class GameObject;

REGISTER_COMPONENT(PointLightComponent, "PointLightComponent")

PointLightComponent::PointLightComponent(glm::vec4 _color, float _intensity, float _ambience,
	float _constant, float _linear, float _quadratic) 
	: LightComponent::LightComponent(_color, _intensity, _ambience)
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

void PointLightComponent::Draw(Shader * shader, int &counter)
{
	shader->setVec3(UNIFORM_NAME + '[' + std::to_string(counter) + "].position", getLightPos());
	shader->setFloat(UNIFORM_NAME + '[' + std::to_string(counter) + "]." + VAR_NAME(constant), constant);
	shader->setFloat(UNIFORM_NAME + '[' + std::to_string(counter) + "]." + VAR_NAME(linear), linear);
	shader->setFloat(UNIFORM_NAME + '[' + std::to_string(counter) + "]." + VAR_NAME(quadratic), quadratic);
	LightComponent::Draw(shader, counter);
}

void PointLightComponent::DrawInspector()
{
	LightComponent::DrawInspector();
	ImGui::InputFloat("Constant", &constant);
	ImGui::InputFloat("Linear", &linear);
	ImGui::InputFloat("Quadratic", &quadratic);
}
