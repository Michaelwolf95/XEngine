#include "LightComponent.h"
#include "GameObject.h"

REGISTER_COMPONENT(LightComponent, "LightComponent")

LightComponent::LightComponent(glm::vec3 _color, float _intensity, int typeID) : Light::Light()
{
	color = _color;
	intensity = _intensity;
	TYPE_ID = typeID;

	gizmoDrawer = std::shared_ptr<GizmoSpriteDrawer>(new GizmoSpriteDrawer("Editor/Gizmos/LightGizmo.png"));
}

LightComponent::~LightComponent() {}

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

int LightComponent::getTypeID()
{
	return TYPE_ID;
}

float LightComponent::getIntensity() {
	return intensity;
}

void LightComponent::OnDrawGizmos()
{
	gizmoDrawer->Draw(this->gameObject->transform->getPosition());
}

void LightComponent::DrawInspector()
{
	ImGui::ColorEdit4("Color", (float*)&color);
	ImGui::InputFloat("Intensity", &intensity);
	if (intensity < 0)
		intensity = 0;
}
