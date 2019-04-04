#include "SpotLightComponent.h"
#include "GameObject.h"
//class GameObject;

REGISTER_COMPONENT(SpotLightComponent, "SpotLightComponent")

SpotLightComponent::SpotLightComponent(glm::vec4 _color, float _intensity, float _ambience,
	float _constant, float _linear, float _quadratic, float _cutOff, float _outerCutOff)
	: LightComponent(_color, _intensity, _ambience)
{
	UNIFORM_NAME = "spotLights";
	TYPE = LightType::SpotLight;
	cutOff = _cutOff;
	outerCutOff = _outerCutOff;
	constant = _constant;
	linear = _linear;
	quadratic = _quadratic;
}

SpotLightComponent::~SpotLightComponent() {}

void SpotLightComponent::Start() {}
void SpotLightComponent::Update() {}

void SpotLightComponent::DrawInspector()
{
	LightComponent::DrawInspector();
	ImGui::SliderFloat("Constant", (float*)&constant, 0.0f, 2.0f);
	ImGui::SliderFloat("Linear", (float*)&linear, 0.0f, 2.0f);
	ImGui::SliderFloat("Quadratic", (float*)&quadratic, 0.0f, 2.0f);
	ImGui::SliderAngle("CutOff", (float*)&cutOff);
	ImGui::SliderAngle("Outer-CutOff", (float*)&outerCutOff);
}

void SpotLightComponent::Draw(Shader * shader, int &counter)
{
	shader->setVec3(UNIFORM_NAME + '[' + std::to_string(counter) + "].source.position", getLightPos());
	shader->setFloat(UNIFORM_NAME + '[' + std::to_string(counter) + "].source." + VAR_NAME(constant), constant);
	shader->setFloat(UNIFORM_NAME + '[' + std::to_string(counter) + "].source." + VAR_NAME(linear), linear);
	shader->setFloat(UNIFORM_NAME + '[' + std::to_string(counter) + "].source." + VAR_NAME(quadratic), quadratic);
	shader->setVec3(UNIFORM_NAME + '[' + std::to_string(counter) + "].direction", getDirection());
	shader->setFloat(UNIFORM_NAME + '[' + std::to_string(counter) + "]." + VAR_NAME(cutOff), cutOff);
	shader->setFloat(UNIFORM_NAME + '[' + std::to_string(counter) + "]." + VAR_NAME(outerCutOff), outerCutOff);
	
	// color and intensity done here for spotlight since different structure
	//LightComponent::Draw(shader, counter);
	shader->setVec4(UNIFORM_NAME + '[' + std::to_string(counter) + "].source." + VAR_NAME(color), color);
	shader->setFloat(UNIFORM_NAME + '[' + std::to_string(counter) + "].source." + VAR_NAME(intensity), intensity);
	shader->setFloat(UNIFORM_NAME + '[' + std::to_string(counter) + "].source." + VAR_NAME(ambience), ambience);
}