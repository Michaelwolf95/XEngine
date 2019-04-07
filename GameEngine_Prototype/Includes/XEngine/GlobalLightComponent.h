#pragma once
#include "Component.h"
#include "Light.h"
#include "LightComponent.h"
#include <glm/glm.hpp>

#define VAR_NAME(Var) (#Var)

class DLLExport GlobalLightComponent : public LightComponent
{
public:
	static Registrar<GlobalLightComponent> registrar;
	static const LightType TYPE_ID = LightType::GlobalLight;

	// direction is a function of transform

	GlobalLightComponent(glm::vec4 _color = glm::vec4(1.0f), float _intensity = 1.0f, float _ambience = 0.1f);

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
	}
};
