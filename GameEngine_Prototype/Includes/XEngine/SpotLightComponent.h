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

	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;

	SpotLightComponent(glm::vec4 _color = glm::vec4(1.0f), float _intensity = 1.0f, float _ambience = 0.1f,
		float _constant = 1.0f, float _linear = 0.09f, float _quadratic = 0.032f,
		float _cutOff = glm::cos(glm::radians(12.5f)), float _outerCutOff = glm::cos(glm::radians(17.5f)));
	~SpotLightComponent();

	void Start() override;
	void Update() override;

	void DrawInspector() override;
	void Draw(Shader* shader, int &counter) override;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		// save/load base class information
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(LightComponent);
		ar & BOOST_SERIALIZATION_NVP(constant);
		ar & BOOST_SERIALIZATION_NVP(linear);
		ar & BOOST_SERIALIZATION_NVP(quadratic);
		ar & BOOST_SERIALIZATION_NVP(cutOff);
	}
};
