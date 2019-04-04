#include "SpotLightComponent.h"
#include "GameObject.h"
//class GameObject;

REGISTER_COMPONENT(SpotLightComponent, "SpotLightComponent")

SpotLightComponent::SpotLightComponent(glm::vec4 _color, float _intensity, 
	float _constant, float _linear, float _quadratic,/* glm::vec3 _initial_direction, */float _cutOff)
	: LightComponent(_color, _intensity)
{
	UNIFORM_NAME = "spotLights";
	TYPE = LightType::SpotLight;
	cutOff = _cutOff;
	constant = _constant;
	linear = _linear;
	quadratic = _quadratic;
}


SpotLightComponent::~SpotLightComponent() {}

void SpotLightComponent::Start() {}
void SpotLightComponent::Update() {}

void SpotLightComponent::Draw(Shader * shader, int &counter)
{
	shader->setVec3(UNIFORM_NAME + '[' + std::to_string(counter) + "].source.position", getLightPos());
	shader->setFloat(UNIFORM_NAME + '[' + std::to_string(counter) + "].source." + VAR_NAME(constant), constant);
	shader->setFloat(UNIFORM_NAME + '[' + std::to_string(counter) + "].source." + VAR_NAME(linear), linear);
	shader->setFloat(UNIFORM_NAME + '[' + std::to_string(counter) + "].source." + VAR_NAME(quadratic), quadratic);
	shader->setVec3(UNIFORM_NAME + '[' + std::to_string(counter) + "].direction", getDirection());
	shader->setFloat(UNIFORM_NAME + '[' + std::to_string(counter) + "]." + VAR_NAME(cutOff), cutOff);
	// color and intensity done here -- except for spotlight since has different structure
	//LightComponent::Draw(shader, counter);
	shader->setVec4(UNIFORM_NAME + '[' + std::to_string(counter) + "].source." + VAR_NAME(color), color);
	shader->setFloat(UNIFORM_NAME + '[' + std::to_string(counter) + "].source." + VAR_NAME(intensity), intensity);
}


//struct PointLight {
//	glm::vec3 color;
//	float intensity;
//	glm::vec3 position;
//	glm::vec3 ambient;
//	glm::vec3 diffuse;
//	glm::vec3 specular;
//
//	float constant;
//	float linear;
//	float quadratic;
//};
//
//struct SpotLight {
//	// shares attributes with PointLight
//	PointLight source;
//
//	// unique attributes
//	glm::vec3 direction;
//	float cutOff;
//};
