#include "LightComponent.h"
#include "GameObject.h"
#include "GizmoSpriteDrawer.h"

//REGISTER_COMPONENT(LightComponent, "LightComponent")

LightComponent::LightComponent(glm::vec3 _color, float _intensity, int typeID) : Light::Light()
{
	color = _color;
	intensity = _intensity;
	gizmoDrawer = std::make_shared<GizmoSpriteDrawer>(GizmoSpriteDrawer("Editor/Gizmos/LightGizmo.png"));
}

LightComponent::~LightComponent() {
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

//glm::vec3 LightComponent::getAmbient()
//{
//	return ambient;
//}
//
//glm::vec3 LightComponent::getDiffuse()
//{
//	return diffuse;
//}
//
//glm::vec3 LightComponent::getSpecular()
//{
//	return specular;
//}

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

void LightComponent::DrawInspector()
{
	ImGui::ColorEdit4("Color", (float*)&color);
	ImGui::InputFloat("Intensity", &intensity);
	if (intensity < 0)
		intensity = 0;
}

void LightComponent::OnDrawGizmos()
{
	gizmoDrawer->Draw(this->gameObject->transform->getPosition());
}

void LightComponent::draw(Shader * shader, int &counter)
{
	std::string uniformString = UNIFORM_NAME + '[' + std::to_string(counter) + "].";
	shader->setVec3(uniformString + VAR_NAME(color), color);
	shader->setFloat(uniformString + VAR_NAME(intensity), intensity);
	//std::cout << uniformString + VAR_NAME(color) << " ==  " << color.x << " " << color.y << " " << color.z << std::endl;
	//std::cout << uniformString + VAR_NAME(intensity) << " == " << intensity << std::endl;
	////counter++;
}

const std::string* LightComponent::getUniformName()
{
	return &UNIFORM_NAME;
}

void LightComponentDraw(Shader* shader) {

}

