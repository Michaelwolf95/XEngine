#include "LightComponent.h"
#include "GameObject.h"
#include "GizmoSpriteDrawer.h"

//REGISTER_COMPONENT(LightComponent, "LightComponent")

LightComponent::LightComponent(glm::vec4 _color, float _intensity, float _ambience) : Light::Light()
{
	color = _color;
	intensity = _intensity;
	ambience = _ambience;
	gizmoDrawer = std::make_shared<GizmoSpriteDrawer>(GizmoSpriteDrawer("Editor/Gizmos/LightGizmo.png"));
}

LightComponent::~LightComponent() {}

void LightComponent::Start() {}
void LightComponent::Update() {}

glm::vec3 LightComponent::getLightPos()
{
	return this->gameObject->transform->getPosition();
}

glm::vec3 LightComponent::getDirection()
{
	return this->gameObject->transform->getForwardDirection();
}

int LightComponent::getTypeID()
{
	return TYPE;
}

const std::string* LightComponent::getUniformName()
{
	return &UNIFORM_NAME;
}

void LightComponent::DrawInspector()
{
	ImGui::ColorEdit4("Color", (float*)&color);
	ImGui::SliderFloat("Intensity", &intensity, 0.0f, 100.0f);
	ImGui::SliderFloat("Ambience", &ambience, 0.0f, 2.0f);
	if (intensity < 0) intensity = 0;
}

void LightComponent::OnDrawGizmos()
{
	gizmoDrawer->Draw(this->gameObject->transform->getPosition());
}

void LightComponent::Draw(Shader * shader, int &counter)
{
	std::string uniformString = UNIFORM_NAME + '[' + std::to_string(counter) + "].";
	shader->setVec4(uniformString + VAR_NAME(color), color);
	shader->setFloat(uniformString + VAR_NAME(intensity), intensity);
	shader->setFloat(uniformString + VAR_NAME(ambience), ambience);
}

