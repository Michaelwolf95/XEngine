#include "GlobalLightComponent.h"
#include "GameObject.h"

REGISTER_COMPONENT(GlobalLightComponent, "GlobalLightComponent")

GlobalLightComponent::GlobalLightComponent(glm::vec4 _color, float _intensity, float _ambience)
	: LightComponent(_color, _intensity, _ambience)
{
	UNIFORM_NAME = "globalLights";
	TYPE = LightType::GlobalLight;
}

void GlobalLightComponent::Start() {}
void GlobalLightComponent::Update() {}

void GlobalLightComponent::DrawInspector()
{
	LightComponent::DrawInspector();
}

void GlobalLightComponent::Draw(Shader * shader, int &counter)
{
	shader->setVec3(UNIFORM_NAME + '[' + std::to_string(counter) + "].direction", getDirection()); // TODO: Only update when necessary (static techique)
	LightComponent::Draw(shader, counter);
}

GlobalLightComponent * GlobalLightComponent::DeepCopy()
{
	return new GlobalLightComponent(*this);
}