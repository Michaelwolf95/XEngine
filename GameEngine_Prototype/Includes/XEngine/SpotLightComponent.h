#pragma once
#include "Component.h"
#include "Light.h"
#include "LightComponent.h"
#include <glm/glm.hpp>
#include <vector>

#define VAR_NAME(Var) (#Var)

class DLLExport SpotLightComponent : public LightComponent
{
public:
	static Registrar<SpotLightComponent> registrar;
	static const LightType TYPE_ID = LightType::SpotLight;

	// position is a function of transform component
	glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);

	float constant;
	float linear;
	float quadratic;
	float cutOff;

	SpotLightComponent(glm::vec4 _color = glm::vec4(1.0f), float _intensity = 1.0f, 
		float _constant = 1.0f, float _linear = 0.09f, float _quadratic = 0.032f,
		float cutOff = glm::cos(glm::radians(12.5f)));
	~SpotLightComponent();

	void Start() override;
	void Update() override;

	void Draw(Shader* shader, int &counter) override;


private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		// save/load base class information
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(LightComponent);
	}
};
